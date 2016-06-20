/**
 *
 */
package rocks.inspectit.shared.cs.ci.sensor.dotNet;

import java.util.Map;

import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlRootElement;

import rocks.inspectit.shared.cs.ci.sensor.StringConstraintSensorConfig;
import rocks.inspectit.shared.cs.ci.sensor.method.IMethodSensorConfig;
import rocks.inspectit.shared.cs.ci.sensor.method.MethodSensorPriorityEnum;

/**
 * @author Henning Schulz
 *
 */
@XmlRootElement(name = "dot-net-stack-trace-sensor-config")
public class DotNetStackTraceSensorConfig extends StringConstraintSensorConfig implements IMethodSensorConfig {

	public static final String SENSOR_NAME = "[.NET] Stack Trace Sampling Sensor";
	public static final String CLASS_NAME = "dotNet.DotNetStackTraceSensor";

	@XmlAttribute(name = "trigger-type", required = true)
	private TriggerType triggerType;
	@XmlAttribute(name = "provider-type", required = true)
	private StackTraceProviderType providerType;
	@XmlAttribute(name = "sampling-interval", required = true)
	private long samplingInterval;

	public DotNetStackTraceSensorConfig() {
		this(TriggerType.TIMER, StackTraceProviderType.SHADOW_STACK, 50);
	}

	public DotNetStackTraceSensorConfig(TriggerType triggerType, StackTraceProviderType providerType, long samplingInterval) {
		super(100);
		this.triggerType = triggerType;
		this.providerType = providerType;
		this.samplingInterval = samplingInterval;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public String getClassName() {
		return CLASS_NAME;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public String getName() {
		return SENSOR_NAME;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public MethodSensorPriorityEnum getPriority() {
		return MethodSensorPriorityEnum.MAX;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public boolean isAdvanced() {
		return true;
	}

	/**
	 * Gets {@link #triggerType}.
	 *
	 * @return {@link #triggerType}
	 */
	public TriggerType getTriggerType() {
		return triggerType;
	}

	/**
	 * Sets {@link #triggerType}.
	 *
	 * @param triggerType
	 *            New value for {@link #triggerType}
	 */
	public void setTriggerType(TriggerType triggerType) {
		this.triggerType = triggerType;
	}

	/**
	 * Gets {@link #providerType}.
	 *
	 * @return {@link #providerType}
	 */
	public StackTraceProviderType getProviderType() {
		return providerType;
	}

	/**
	 * Sets {@link #providerType}.
	 *
	 * @param providerType
	 *            New value for {@link #providerType}
	 */
	public void setProviderType(StackTraceProviderType providerType) {
		this.providerType = providerType;
	}

	/**
	 * Gets {@link #samplingInterval}.
	 *
	 * @return {@link #samplingInterval}
	 */
	public long getSamplingInterval() {
		return samplingInterval;
	}

	/**
	 * Sets {@link #samplingInterval}.
	 *
	 * @param samplingInterval
	 *            New value for {@link #samplingInterval}
	 */
	public void setSamplingInterval(long samplingInterval) {
		this.samplingInterval = samplingInterval;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public Map<String, Object> getParameters() {
		Map<String, Object> params = super.getParameters();

		if (triggerType != null) {
			params.put("triggerType", triggerType);
		}

		if (providerType != null) {
			params.put("providerType", providerType);
		}

		params.put("sampling-interval", samplingInterval);

		return params;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + getStringLength();
		result = prime * result + triggerType.hashCode();
		result = prime * result + providerType.hashCode();
		result = prime * result + (int) samplingInterval;
		return result;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public boolean equals(Object obj) {
		if (this == obj) {
			return true;
		}
		if (obj == null) {
			return false;
		}
		if (getClass() != obj.getClass()) {
			return false;
		}

		DotNetStackTraceSensorConfig other = (DotNetStackTraceSensorConfig) obj;
		if (this.getStringLength() != other.getStringLength()) {
			return false;
		}
		if (this.triggerType != other.triggerType) {
			return false;
		}
		if (this.providerType != other.providerType) {
			return false;
		}
		if (this.samplingInterval != other.samplingInterval) {
			return false;
		}
		return true;
	}

	public static enum TriggerType {
		TIMER("Timer", true), RANDOMIZED_TIMER("Randomized Timer", true), STACK_SIZE_BASED("Stack Size Based", false);

		private final String name;

		private final boolean dependsOnSamplingInterval;

		private TriggerType(String name, boolean dependsOnSamplingInterval) {
			this.name = name;
			this.dependsOnSamplingInterval = dependsOnSamplingInterval;
		}

		/**
		 * Gets {@link #name}.
		 *
		 * @return {@link #name}
		 */
		public String getName() {
			return name;
		}

		public boolean dependsOnSamplingInterval() {
			return dependsOnSamplingInterval;
		}
	}

	public static enum StackTraceProviderType {
		SIMPLE("Native Stack"), SHADOW_STACK("Shadow Stack");

		private final String name;

		private StackTraceProviderType(String name) {
			this.name = name;
		}

		/**
		 * Gets {@link #name}.
		 *
		 * @return {@link #name}
		 */
		public String getName() {
			return name;
		}
	}

}
