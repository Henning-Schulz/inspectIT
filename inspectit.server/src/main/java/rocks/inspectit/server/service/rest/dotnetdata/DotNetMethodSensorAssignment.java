/**
 *
 */
package rocks.inspectit.server.service.rest.dotnetdata;

import java.util.ArrayList;
import java.util.List;

import rocks.inspectit.shared.cs.ci.assignment.impl.DotNetStackTraceSensorAssignment;
import rocks.inspectit.shared.cs.ci.assignment.impl.MethodSensorAssignment;
import rocks.inspectit.shared.cs.ci.exclude.ExcludeRule;

/**
 * @author Henning Schulz
 *
 */
public class DotNetMethodSensorAssignment extends DotNetStackTraceSensorAssignment {

	private List<String> excludedClasses;

	/**
	 *
	 */
	public DotNetMethodSensorAssignment() {
	}

	public DotNetMethodSensorAssignment(MethodSensorAssignment other) {
		this.setAnnotation(other.getAnnotation());
		this.setClassName(other.getClassName());
		this.setConstructor(other.isConstructor());
		this.setDefaultModifier(other.isDefaultModifier());
		this.setInterf(other.isInterf());
		this.setMethodName(other.getMethodName());
		this.setParameters(other.getParameters());
		this.setPrivateModifier(other.isPrivateModifier());
		this.setProtectedModifier(other.isProtectedModifier());
		this.setPublicModifier(other.isPublicModifier());
		this.setSuperclass(other.isSuperclass());

		if (other instanceof DotNetStackTraceSensorAssignment) {
			this.setBaseMethodAssignment(((DotNetStackTraceSensorAssignment) other).getBaseMethodAssignment());
		}
	}

	/**
	 * @param assignment
	 * @param excludeRules
	 */
	public DotNetMethodSensorAssignment(List<ExcludeRule> excludeRules) {
		this.excludedClasses = new ArrayList<>(excludeRules.size());
		for (ExcludeRule rule : excludeRules) {
			excludedClasses.add(rule.getClassName());
		}
	}

	public DotNetMethodSensorAssignment(MethodSensorAssignment other, List<ExcludeRule> excludeRules) {
		this(other);
		this.excludedClasses = new ArrayList<>(excludeRules.size());
		for (ExcludeRule rule : excludeRules) {
			excludedClasses.add(rule.getClassName());
		}
	}

	/**
	 * Gets {@link #excludedClasses}.
	 *
	 * @return {@link #excludedClasses}
	 */
	public List<String> getExcludedClasses() {
		return excludedClasses;
	}

	/**
	 * Sets {@link #excludedClasses}.
	 *
	 * @param excludedClasses
	 *            New value for {@link #excludedClasses}
	 */
	public void setExcludedClasses(List<String> excludedClasses) {
		this.excludedClasses = excludedClasses;
	}

}
