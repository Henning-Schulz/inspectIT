/**
 *
 */
package rocks.inspectit.server.service.rest;

import java.io.IOException;

import javax.xml.bind.JAXBException;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import rocks.inspectit.server.ci.ConfigurationInterfaceManager;
import rocks.inspectit.shared.all.exception.BusinessException;
import rocks.inspectit.shared.cs.ci.AgentMapping;
import rocks.inspectit.shared.cs.ci.AgentMappings;
import rocks.inspectit.shared.cs.ci.Environment;
import rocks.inspectit.shared.cs.ci.sensor.dotNet.DotNetStackTraceSensorConfig;
import rocks.inspectit.shared.cs.ci.sensor.dotNet.DotNetStackTraceSensorConfig.StackTraceProviderType;
import rocks.inspectit.shared.cs.ci.sensor.dotNet.DotNetStackTraceSensorConfig.TriggerType;
import rocks.inspectit.shared.cs.ci.sensor.method.IMethodSensorConfig;

/**
 * @author Henning Schulz
 *
 */
@Controller
@RequestMapping(value = "/dotNetConfigurationInterface")
public class DotNetConfigurationInterfaceService {

	@Autowired
	private ConfigurationInterfaceManager ciManager;

	@RequestMapping(method = RequestMethod.GET, value = "setStackTraceSensorConfig")
	@ResponseBody
	public void setStackTraceSensorConfig(@RequestParam(required = true, value = "triggerType") TriggerType triggerType,
			@RequestParam(required = true, value = "providerType") StackTraceProviderType providerType, @RequestParam(required = true, value = "samplingInterval") long samplingInterval)
			throws BusinessException, JAXBException, IOException {
		AgentMappings mappings = ciManager.getAgentMappings();
		Environment environment = null;
		String agentName = ".NET agent";

		for (AgentMapping map : mappings.getMappings()) {
			if (map.isActive() && agentName.equals(map.getAgentName())) {
				try {
					environment = ciManager.getEnvironment(map.getEnvironmentId());
					break;
				} catch (BusinessException e) {
					e.printStackTrace();
				}
			}
		}

		for (IMethodSensorConfig config : environment.getMethodSensorConfigs()) {
			if (config instanceof DotNetStackTraceSensorConfig) {
				DotNetStackTraceSensorConfig stsConfig = (DotNetStackTraceSensorConfig) config;
				stsConfig.setTriggerType(triggerType);
				stsConfig.setProviderType(providerType);
				stsConfig.setSamplingInterval(samplingInterval);
			}
		}

		ciManager.updateEnvironment(environment, true);
	}

}
