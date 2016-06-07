/**
 *
 */
package rocks.inspectit.server.service.rest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import rocks.inspectit.server.service.rest.dotnetdata.DotNetMethodSensorData;
import rocks.inspectit.shared.all.cmr.service.IAgentStorageService;

/**
 * @author Henning Schulz
 *
 */
@Controller
@RequestMapping(value = "/dotNetAgentStorage")
public class DotNETAgentStorageRestfulService {

	/**
	 * Reference to the existing {@link IAgentStorageService}.
	 */
	@Autowired
	IAgentStorageService agentStorageService;

	/**
	 *
	 * @param dataObjects
	 *            data objects
	 * @return response
	 */
	@RequestMapping(method = RequestMethod.POST, value = "addDataObjects")
	@ResponseBody
	public int addDataObjects(@RequestBody(required = true) DotNetMethodSensorData[] dataObjects) {
		// agentStorageService.addDataObjects(dataObjects);
		System.out.println("Data received. Size was " + dataObjects.length + " and the content is:");

		for (DotNetMethodSensorData data : dataObjects) {
			System.out.println(data);
		}

		return 1;
	}

}
