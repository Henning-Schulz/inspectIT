package rocks.inspectit.server.service.rest;

import java.util.Arrays;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import rocks.inspectit.server.instrumentation.NextGenInstrumentationManager;
import rocks.inspectit.server.messaging.AgentMessageProvider;
import rocks.inspectit.server.service.rest.data.TransferType;
import rocks.inspectit.shared.all.communication.message.IAgentMessage;
import rocks.inspectit.shared.all.exception.BusinessException;
import rocks.inspectit.shared.all.instrumentation.config.impl.AgentConfig;
import rocks.inspectit.shared.all.instrumentation.config.impl.InstrumentationDefinition;
import rocks.inspectit.shared.all.spring.logger.Log;

/**
 * @author Henning Schulz
 *
 */
@Controller
@RequestMapping(value = "/agent")
public class AgentRestfulService {

	/**
	 * Logger for the class.
	 */
	@Log
	Logger log;

	/**
	 * {@link NextGenInstrumentationManager}.
	 */
	@Autowired
	NextGenInstrumentationManager nextGenInstrumentationManager;

	/**
	 * The {@link AgentMessageProvider}.
	 */
	@Autowired
	AgentMessageProvider messageProvider;

	@RequestMapping(method = RequestMethod.POST, value = "register/{agentName}/{version}")
	@ResponseBody
	public AgentConfig register(@RequestBody String[] definedIPs, @PathVariable String agentName, @PathVariable String version) throws BusinessException {
		return nextGenInstrumentationManager.register(Arrays.asList(definedIPs), agentName, version);
	}

	@RequestMapping(method = RequestMethod.DELETE, value = "{platformIdent}/unregister")
	@ResponseBody
	public String unregister(@PathVariable long platformIdent) throws BusinessException {
		nextGenInstrumentationManager.unregister(platformIdent);
		return "Unregistered platform " + platformIdent + ".";
	}

	@RequestMapping(method = RequestMethod.POST, value = "{platformIdent}/analyze")
	@ResponseBody
	public InstrumentationDefinition analyze(@PathVariable long platformIdent, @RequestBody TransferType transferType)
			throws BusinessException {
		return nextGenInstrumentationManager.analyze(platformIdent, transferType.getHash(), transferType.toType());
	}

	@RequestMapping(method = RequestMethod.POST, value = "{platformIdent}/instrumentation-applied")
	@ResponseBody
	public String instrumentationApplied(@PathVariable long platformIdent, @RequestBody Map<Long, long[]> methodToSensorMap) {
		nextGenInstrumentationManager.instrumentationApplied(platformIdent, methodToSensorMap);
		System.out.println("Instrumentation application stored: " + methodToSensorMap);
		return "Instrumentation application stored.";
	}

	@RequestMapping(method = RequestMethod.GET, value = "{platformIdent}/messages")
	@ResponseBody
	public List<IAgentMessage<?>> fetchAgentMessages(@PathVariable long platformIdent) {
		return messageProvider.fetchMessages(platformIdent);
	}

}
