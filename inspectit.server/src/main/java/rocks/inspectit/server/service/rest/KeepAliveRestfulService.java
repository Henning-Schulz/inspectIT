package rocks.inspectit.server.service.rest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import rocks.inspectit.server.util.AgentStatusDataProvider;

/**
 * @author Henning Schulz
 *
 */
@Controller
@RequestMapping(value = "/keep-alive")
public class KeepAliveRestfulService {

	/**
	 * {@link AgentStatusDataProvider}.
	 */
	@Autowired
	AgentStatusDataProvider agentStatusDataProvider;

	@RequestMapping(method = RequestMethod.POST, value = "{platformId}")
	@ResponseBody
	public String sendKeepAlive(@PathVariable long platformId) {
		agentStatusDataProvider.handleKeepAliveSignal(platformId);
		return "Platform " + platformId + " kept alive.";
	}

}
