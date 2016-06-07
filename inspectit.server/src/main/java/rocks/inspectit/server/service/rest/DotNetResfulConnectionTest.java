/**
 *
 */
package rocks.inspectit.server.service.rest;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

/**
 * @author Henning Schulz
 *
 */
@Controller
@RequestMapping(value = "/dotNetConnection")
public class DotNetResfulConnectionTest {

	@RequestMapping(method = RequestMethod.GET, value = "test")
	@ResponseBody
	public boolean testConnection() {
		System.out.println("Connection tested.");
		return true;
	}

}
