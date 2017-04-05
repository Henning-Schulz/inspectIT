package rocks.inspectit.server.service.rest.data;

import java.util.Arrays;

import rocks.inspectit.shared.all.instrumentation.classcache.AnnotationType;
import rocks.inspectit.shared.all.instrumentation.classcache.ClassType;
import rocks.inspectit.shared.all.instrumentation.classcache.MethodType;

/**
 * @author Henning Schulz
 *
 */
public class TransferMethodType {

	/**
	 * The name of the method.
	 */
	private String name;

	/**
	 * The modifiers of the method.
	 */
	private int modifiers;

	/**
	 * Return type of the method.
	 */
	private String returnType;

	/**
	 * Ordered list of all parameters of this method.
	 */
	private String[] parameters = null;

	/**
	 * List of all exceptions this method throws.
	 */
	private String[] exceptions = null;

	/**
	 * List of annotations of this method.
	 */
	private String[] annotations = null;

	/**
	 * Gets {@link #name}.
	 *
	 * @return {@link #name}
	 */
	public String getName() {
		return this.name;
	}

	/**
	 * Sets {@link #name}.
	 *
	 * @param name
	 *            New value for {@link #name}
	 */
	public void setName(String name) {
		this.name = name;
	}

	/**
	 * Gets {@link #modifiers}.
	 *
	 * @return {@link #modifiers}
	 */
	public int getModifiers() {
		return this.modifiers;
	}

	/**
	 * Sets {@link #modifiers}.
	 *
	 * @param modifiers
	 *            New value for {@link #modifiers}
	 */
	public void setModifiers(int modifiers) {
		this.modifiers = modifiers;
	}

	/**
	 * Gets {@link #returnType}.
	 *
	 * @return {@link #returnType}
	 */
	public String getReturnType() {
		return this.returnType;
	}

	/**
	 * Sets {@link #returnType}.
	 *
	 * @param returnType
	 *            New value for {@link #returnType}
	 */
	public void setReturnType(String returnType) {
		this.returnType = returnType;
	}

	/**
	 * Gets {@link #parameters}.
	 *
	 * @return {@link #parameters}
	 */
	public String[] getParameters() {
		return this.parameters;
	}

	/**
	 * Sets {@link #parameters}.
	 *
	 * @param parameters
	 *            New value for {@link #parameters}
	 */
	public void setParameters(String[] parameters) {
		this.parameters = parameters;
	}

	/**
	 * Gets {@link #exceptions}.
	 *
	 * @return {@link #exceptions}
	 */
	public String[] getExceptions() {
		return this.exceptions;
	}

	/**
	 * Sets {@link #exceptions}.
	 *
	 * @param exceptions
	 *            New value for {@link #exceptions}
	 */
	public void setExceptions(String[] exceptions) {
		this.exceptions = exceptions;
	}

	/**
	 * Gets {@link #annotations}.
	 *
	 * @return {@link #annotations}
	 */
	public String[] getAnnotations() {
		return this.annotations;
	}

	/**
	 * Sets {@link #annotations}.
	 *
	 * @param annotations
	 *            New value for {@link #annotations}
	 */
	public void setAnnotations(String[] annotations) {
		this.annotations = annotations;
	}

	public MethodType toType() {
		MethodType type = new MethodType();

		type.setName(name);
		type.setModifiers(modifiers);
		type.setReturnType(returnType);
		if (null != parameters) {
			type.setParameters(Arrays.asList(parameters));
		}

		if (null != exceptions) {
			for (String exception : exceptions) {
				type.addException(new ClassType(exception));
			}
		}

		if (null != annotations) {
			for (String annotation : annotations) {
				type.addAnnotation(new AnnotationType(annotation));
			}
		}

		return type;
	}

}
