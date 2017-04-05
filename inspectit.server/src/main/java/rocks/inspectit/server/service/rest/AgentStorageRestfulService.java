package rocks.inspectit.server.service.rest;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import rocks.inspectit.server.service.rest.data.TransferData;
import rocks.inspectit.shared.all.cmr.service.IAgentStorageService;
import rocks.inspectit.shared.all.communication.DefaultData;

/**
 * @author Henning Schulz
 *
 */
@Controller
@RequestMapping(value = "/agent-storage")
public class AgentStorageRestfulService {

	/**
	 * Reference to the existing {@link IAgentStorageService}.
	 */
	@Autowired
	IAgentStorageService agentStorageService;

	@RequestMapping(method = RequestMethod.POST, value = "{platformIdent}/{sensorTypeIdent}")
	@ResponseBody
	public String addDataObjects(@PathVariable long platformIdent, @PathVariable long sensorTypeIdent, @RequestBody TransferData[] data) {
		if (null == data) {
			return "Data was null!";
		} else {
			List<DefaultData> defaultData = new ArrayList<>(data.length);

			for (TransferData d : data) {
				defaultData.add(d.toDefaultData(platformIdent, sensorTypeIdent));
			}

			agentStorageService.addDataObjects(defaultData);
			return "Added " + data.length + " data objects.";
		}
	}

}
