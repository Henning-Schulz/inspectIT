/**
 *
 */
package rocks.inspectit.shared.cs.ci.assignment.impl;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;

import rocks.inspectit.shared.cs.ci.sensor.dotNet.DotNetStackTraceSensorConfig;

/**
 * @author Henning Schulz
 *
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlRootElement(name = "dot-net-stack-trace-sensor-assignment")
public class DotNetStackTraceSensorAssignment extends MethodSensorAssignment {

	/**
	 * Describes the assignment to the base methods.
	 */
	@XmlElement(name = "base-method-assignment")
	private MethodSensorAssignment baseMethodAssignment = new MethodSensorAssignment(DotNetStackTraceSensorConfig.class);

	/**
	 *
	 */
	public DotNetStackTraceSensorAssignment() {
		super(DotNetStackTraceSensorConfig.class);
	}

	/**
	 * Gets {@link #baseMethodAssignment}.
	 *
	 * @return {@link #baseMethodAssignment}
	 */
	public MethodSensorAssignment getBaseMethodAssignment() {
		return baseMethodAssignment;
	}

	/**
	 * Sets {@link #baseMethodAssignment}.
	 *
	 * @param baseMethodAssignment
	 *            New value for {@link #baseMethodAssignment}
	 */
	public void setBaseMethodAssignment(MethodSensorAssignment baseMethodAssignment) {
		this.baseMethodAssignment = baseMethodAssignment;
	}

}
