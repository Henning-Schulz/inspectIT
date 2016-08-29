/**
 *
 */
package rocks.inspectit.server.service.rest;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import rocks.inspectit.server.ci.ConfigurationInterfaceManager;
import rocks.inspectit.server.dao.PlatformIdentDao;
import rocks.inspectit.shared.all.cmr.model.PlatformIdent;
import rocks.inspectit.shared.all.exception.BusinessException;
import rocks.inspectit.shared.cs.ci.AgentMapping;
import rocks.inspectit.shared.cs.ci.AgentMappings;
import rocks.inspectit.shared.cs.ci.Environment;
import rocks.inspectit.shared.cs.ci.Profile;
import rocks.inspectit.shared.cs.ci.assignment.impl.MethodSensorAssignment;
import rocks.inspectit.shared.cs.ci.exclude.ExcludeRule;
import rocks.inspectit.shared.cs.ci.sensor.method.IMethodSensorConfig;
import rocks.inspectit.shared.cs.ci.strategy.IStrategyConfig;

/**
 * @author Henning Schulz
 *
 */
@Controller
@RequestMapping(value = "/dotNetAgentConfig")
public class DotNetAgentConfigRestfulService {

	private static final long CACHE_EXPIRATION_MS = 2000;

	@Autowired
	private ConfigurationInterfaceManager ciManager;

	@Autowired
	PlatformIdentDao platformIdentDao;

	private final Map<Long, CachedEnvironment> cachedEnvironments = new ConcurrentHashMap<>();

	@RequestMapping(method = RequestMethod.GET, value = "getSendingStrategyConfig")
	@ResponseBody
	public IStrategyConfig getSendingStrategyConfig(@RequestParam(required = true, value = "platformId") long platformId) {
		Environment env = getEnvironment(platformId);

		if (env == null) {
			return null;
		}

		return env.getSendingStrategyConfig();
	}

	@RequestMapping(method = RequestMethod.GET, value = "getBufferStrategyConfig")
	@ResponseBody
	public IStrategyConfig getBufferStrategyConfig(@RequestParam(required = true, value = "platformId") long platformId) {
		Environment env = getEnvironment(platformId);

		if (env == null) {
			System.out.println("Buffer strategy is null!");
			return null;
		}

		return env.getBufferStrategyConfig();
	}

	@RequestMapping(method = RequestMethod.GET, value = "getMethodSensorConfigs")
	@ResponseBody
	public List<IMethodSensorConfig> getMethodSensorConfigs(@RequestParam(required = true, value = "platformId") long platformId) {
		Environment env = getEnvironment(platformId);

		if (env == null) {
			return null;
		}

		return env.getMethodSensorConfigs();
	}

	@RequestMapping(method = RequestMethod.GET, value = "getProfiles")
	@ResponseBody
	public List<Profile> getProfiles(@RequestParam(required = true, value = "platformId") long platformId) {
		Environment env = getEnvironment(platformId);

		if (env == null) {
			return null;
		}

		List<Profile> profiles = new ArrayList<>();

		for (String id : env.getProfileIds()) {
			try {
				profiles.add(ciManager.getProfile(id));
			} catch (BusinessException e) {
				e.printStackTrace();
			}
		}

		return profiles;
	}

	@RequestMapping(method = RequestMethod.GET, value = "getSensorAssignments")
	@ResponseBody
	public List<MethodSensorAssignment> getSensorAssignments(@RequestParam(required = true, value = "platformId") long platformId) {
		List<Profile> profiles = getProfiles(platformId);

		if (profiles == null) {
			return null;
		}

		List<MethodSensorAssignment> assignments = new ArrayList<>();

		for (Profile p : profiles) {
			if (p.getMethodSensorAssignments() != null) {
				assignments.addAll(p.getMethodSensorAssignments());
			}
		}

		return assignments;
	}

	@RequestMapping(method = RequestMethod.GET, value = "getExcludedClasses")
	@ResponseBody
	public List<String> getExcludedClasses(@RequestParam(required = true, value = "platformId") long platformId) {
		List<Profile> profiles = getProfiles(platformId);

		if (profiles == null) {
			return null;
		}

		List<String> excludedClasses = new ArrayList<>();

		for (Profile p : profiles) {
			if (p.getExcludeRules() != null) {
				for (ExcludeRule exclude : p.getExcludeRules()) {
					excludedClasses.add(exclude.getClassName());
				}
			}
		}

		return excludedClasses;
	}

	private Environment getEnvironment(long platformId) {
		CachedEnvironment env = cachedEnvironments.get(platformId);

		if (env == null || !env.isValid()) {
			env = null;
			PlatformIdent pi = platformIdentDao.load(platformId);

			if (pi == null) {
				return null;
			}

			String agentName = pi.getAgentName();

			AgentMappings mappings = ciManager.getAgentMappings();

			if (mappings == null || mappings.getMappings() == null || mappings.getMappings().isEmpty()) {
				return null;
			}

			for (AgentMapping map : mappings.getMappings()) {
				if (map.isActive() && agentName.equals(map.getAgentName())) {
					try {
						Environment retrieved = ciManager.getEnvironment(map.getEnvironmentId());
						env = new CachedEnvironment(retrieved);
						cachedEnvironments.put(platformId, env);
						break;
					} catch (BusinessException e) {
						e.printStackTrace();
					}
				}
			}
		}

		return env.getEnvironment();
	}

	private static class CachedEnvironment {

		private final Environment environment;
		private final long expriation;

		CachedEnvironment(Environment environment) {
			this.environment = environment;
			this.expriation = System.currentTimeMillis() + CACHE_EXPIRATION_MS;
		}

		/**
		 * Gets {@link #environment}.
		 *
		 * @return {@link #environment}
		 */
		public Environment getEnvironment() {
			return environment;
		}

		/**
		 *
		 */
		private boolean isValid() {
			long curr = System.currentTimeMillis();
			return curr < expriation;
		}

	}

}
