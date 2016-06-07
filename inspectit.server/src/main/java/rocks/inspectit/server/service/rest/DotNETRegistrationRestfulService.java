/**
 *
 */
package rocks.inspectit.server.service.rest;

import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import rocks.inspectit.shared.all.cmr.service.IRegistrationService;
import rocks.inspectit.shared.all.exception.BusinessException;

/**
 * @author Henning Schulz
 *
 */
@Controller
@RequestMapping(value = "/dotNetRegistration")
public class DotNETRegistrationRestfulService {

	/**
	 * Reference to the existing {@link IRegistrationService}.
	 */
	@Autowired
	IRegistrationService registrationService;

	@RequestMapping(method = RequestMethod.POST, value = "registerPlatformIdent")
	@ResponseBody
	public long registerPlatformIdent(@RequestParam(value = "agentName", required = true) String agentName, @RequestParam(value = "version", required = true) String version,
			@RequestBody(required = true) List<String> definedIPs) {
		System.out.println("registerPlatformIdent(agentName = " + agentName + ", version = " + version + ", definedIPs = " + definedIPs + ")");

		if (agentName == null || version == null) {
			return -1;
		}

		if (definedIPs == null) {
			definedIPs = new ArrayList<>();
		}

		try {
			return registrationService.registerPlatformIdent(definedIPs, agentName, version);
		} catch (BusinessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return -1;
		}
	}

	@RequestMapping(method = RequestMethod.POST, value = "unregisterPlatformIdent")
	@ResponseBody
	public int unregisterPlatformIdent(@RequestParam(value = "agentName", required = true) String agentName, @RequestBody(required = true) List<String> definedIPs) {
		System.out.println("unregisterPlatformIdent(agentName = " + agentName + ", definedIPs = " + definedIPs + ")");

		if (agentName == null) {
			return -1;
		}

		if (definedIPs == null) {
			definedIPs = new ArrayList<>();
		}

		try {
			registrationService.unregisterPlatformIdent(definedIPs, agentName);
			return 1;
		} catch (BusinessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return -1;
		}
	}

	@RequestMapping(method = RequestMethod.POST, value = "registerMethodIdent")
	@ResponseBody
	public long registerMethodIdent(@RequestParam(value = "platformId", required = true) long platformId, @RequestParam(value = "className", required = true) String className,
			@RequestParam(value = "methodName", required = true) String methodName, @RequestParam(value = "returnType", required = true) String returnType,
			@RequestParam(value = "modifiers", required = true) int modifiers, @RequestBody(required = true) List<String> parameterTypes) {
		if (className == null) {
			// error
			System.err.println("className is null");

			return -1;
		}

		int pointIdx = className.lastIndexOf(".");
		String packageName;
		if (pointIdx > 0) {
			packageName = className.substring(0, pointIdx);
			className = className.substring(pointIdx + 1);
		} else {
			packageName = "";
		}

		String signature = Modifier.toString(modifiers) + " " + returnType + " " + packageName + "." + className + "." + methodName + "(";
		if (parameterTypes != null) {
			boolean first = true;
			for (String param : parameterTypes) {
				if (first) {
					first = false;
				} else {
					signature += ", ";
				}

				signature += param;
			}
		}
		signature += ")";

		System.out.println("registerMethodIdent on platform " + platformId + " for method " + signature);

		long id = registrationService.registerMethodIdent(platformId, packageName, className, methodName, parameterTypes, returnType, modifiers);
		System.out.println("ID is " + id);
		return id;
	}

	@RequestMapping(method = RequestMethod.POST, value = "registerMethodSensorTypeIdent")
	@ResponseBody
	public long registerMethodSensorTypeIdent(@RequestParam(value = "platformId", required = true) long platformId,
			@RequestParam(value = "fullyQualifiedClassName", required = true) String fullyQualifiedClassName) {
		System.out.println("registerMethodSensorTypeIdent(platformId = " + platformId + ", fullyQualifiedClassName = " + fullyQualifiedClassName + ")");

		return registrationService.registerMethodSensorTypeIdent(platformId, fullyQualifiedClassName, null);
	}

	@RequestMapping(method = RequestMethod.POST, value = "addSensorTypeToMethod")
	@ResponseBody
	public void addSensorTypeToMethod(@RequestParam(value = "methodSensorTypeId", required = true) long methodSensorTypeId, @RequestParam(value = "methodId", required = true) long methodId) {
		System.out.println("addSensorTypeToMethod(methodSensorTypeId = " + methodSensorTypeId + ", methodId = " + methodId + ")");

		registrationService.addSensorTypeToMethod(methodSensorTypeId, methodId);
	}

}
