/**
 *
 */
package rocks.inspectit.server.service.rest;

import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import rocks.inspectit.server.service.rest.dotnetdata.DotNetMethodSensorData;
import rocks.inspectit.shared.all.cmr.service.IAgentStorageService;
import rocks.inspectit.shared.all.cmr.service.IKeepAliveService;
import rocks.inspectit.shared.all.communication.DefaultData;
import rocks.inspectit.shared.all.communication.data.InvocationSequenceData;

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

	@Autowired
	IKeepAliveService keepAliveService;

	/**
	 *
	 * @param dataObjects
	 *            data objects
	 * @return response
	 */
	@RequestMapping(method = RequestMethod.POST, value = "addDataObjects")
	@ResponseBody
	public int addDataObjects(@RequestBody(required = true) DotNetMethodSensorData[] dataObjects) {
		// System.out.println("Data recieved. Size is " + dataObjects.length + ".");
		List<DefaultData> defaultDataList = new ArrayList<>();

		for (DotNetMethodSensorData data : dataObjects) {
			// System.out.println(data);
			defaultDataList.add(data.toDefaultData());

			// if (data.isInvocationSequenceConvertible()) {
			// InvocationSequenceData invocData;
			// try {
			// invocData = data.toInvocationSequenceData();
			// } catch (IllegalArgumentException e) {
			// e.printStackTrace();
			// invocData = null;
			// }
			// if (invocData != null) {
			// System.out.println("As InvocationSequenceData:");
			// printInvocData(invocData, "");
			// System.out.println();
			// // defaultDataList.add(invocData);
			// }
			// }

		}

		agentStorageService.addDataObjects(defaultDataList);

		return 1;
	}

	@RequestMapping(method = RequestMethod.POST, value = "keepAlive")
	@ResponseBody
	public int keepAlive(@RequestParam(value = "platformId", required = true) long platformId) {
		// System.out.println("Keep platform " + platformId + " alive");
		keepAliveService.sendKeepAlive(platformId);
		return 1;
	}

	private void printInvocData(InvocationSequenceData invocData, String prefix) {
		DecimalFormat format = new DecimalFormat("00000");
		System.out.print(format.format(invocData.getMethodIdent()) + " ");

		boolean first = true;

		for (InvocationSequenceData child : invocData.getNestedSequences()) {
			if (first) {
				first = false;
			} else {
				System.out.println();
				System.out.print(prefix + "      ");
			}

			printInvocData(child, prefix + "      ");
		}
	}

}
