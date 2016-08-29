/**
 *
 */
package rocks.inspectit.server.service.rest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import rocks.inspectit.shared.cs.cmr.service.ICmrManagementService;

/**
 * @author Henning Schulz
 *
 */
@Controller
@RequestMapping(value = "/cmrManagement")
public class CmrManagementRestfulService {

	@Autowired
	ICmrManagementService managementService;

	@RequestMapping(method = RequestMethod.GET, value = "restart")
	@ResponseBody
	public void restart() {
		managementService.restart();
	}

	@RequestMapping(method = RequestMethod.GET, value = "testConnection")
	@ResponseBody
	public boolean testConnection() {
		return true;
	}

}
