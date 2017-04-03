/**
 *
 */
package rocks.inspectit.server.service.rest;

import org.slf4j.Logger;

import rocks.inspectit.server.spring.aop.MethodLog;
import rocks.inspectit.shared.all.exception.BusinessException;
import rocks.inspectit.shared.all.instrumentation.classcache.ClassType;
import rocks.inspectit.shared.all.instrumentation.config.impl.InstrumentationDefinition;
import rocks.inspectit.shared.all.spring.logger.Log;

/**
 * @author Henning Schulz
 *
 */
public class AgentResfulService {

	/**
	 * Logger for the class.
	 */
	@Log
	Logger log;

	@MethodLog
	public InstrumentationDefinition analyze(long platformIdent, String hash, String packageName, String className, int modifiers) throws BusinessException {
		ClassType type = new ClassType(packageName + "." + className, hash, modifiers);
	}

}
