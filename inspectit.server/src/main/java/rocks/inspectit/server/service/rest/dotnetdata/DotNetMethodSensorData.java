/**
 *
 */
package rocks.inspectit.server.service.rest.dotnetdata;

import java.sql.Timestamp;

import org.codehaus.jackson.annotate.JsonSubTypes;
import org.codehaus.jackson.annotate.JsonTypeInfo;

import rocks.inspectit.shared.all.communication.DefaultData;
import rocks.inspectit.shared.all.communication.MethodSensorData;
import rocks.inspectit.shared.all.communication.data.InvocationSequenceData;

/**
 * @author Henning Schulz
 *
 */
@JsonTypeInfo(use = JsonTypeInfo.Id.NAME, include = JsonTypeInfo.As.PROPERTY, property = "objectType")
@JsonSubTypes({ @JsonSubTypes.Type(name = "HelloData", value = DotNetHelloData.class), @JsonSubTypes.Type(name = "StackTraceData", value = DotNetStackTraceData.class),
		@JsonSubTypes.Type(name = "TimerData", value = DotNetTimerData.class) })
public abstract class DotNetMethodSensorData {

	/**
	 * A timestamp in milliseconds since 01.01.1970.
	 */
	private long timestamp;

	/**
	 * The ID of the platform.
	 */
	private long platformId;

	/**
	 * The ID of the sensor type.
	 */
	private long methodSensorId;

	/**
	 * The ID of the method.
	 */
	private long methodId;

	/**
	 * Gets {@link #timestamp}.
	 *
	 * @return {@link #timestamp}
	 */
	public long getTimestamp() {
		return timestamp;
	}

	/**
	 * Sets {@link #timestamp}.
	 *
	 * @param timestamp
	 *            New value for {@link #timestamp}
	 */
	public void setTimestamp(long timestamp) {
		this.timestamp = timestamp;
	}

	/**
	 * Gets {@link #platformId}.
	 *
	 * @return {@link #platformId}
	 */
	public long getPlatformId() {
		return platformId;
	}

	/**
	 * Sets {@link #platformId}.
	 *
	 * @param platformId
	 *            New value for {@link #platformId}
	 */
	public void setPlatformId(long platformId) {
		this.platformId = platformId;
	}

	/**
	 * Gets {@link #methodSensorId}.
	 *
	 * @return {@link #methodSensorId}
	 */
	public long getMethodSensorId() {
		return methodSensorId;
	}

	/**
	 * Sets {@link #methodSensorId}.
	 *
	 * @param methodSensorId
	 *            New value for {@link #methodSensorId}
	 */
	public void setMethodSensorId(long methodSensorId) {
		this.methodSensorId = methodSensorId;
	}

	/**
	 * Gets {@link #methodId}.
	 *
	 * @return {@link #methodId}
	 */
	public long getMethodId() {
		return methodId;
	}

	/**
	 * Sets {@link #methodId}.
	 *
	 * @param methodId
	 *            New value for {@link #methodId}
	 */
	public void setMethodId(long methodId) {
		this.methodId = methodId;
	}

	/**
	 * Converts this .NET data into standard {@link DefaultData}.
	 *
	 * @return equivalent DefaultData
	 */
	public DefaultData toDefaultData() {
		MethodSensorData data = toNewDefaultData();
		addAttributesToDefaultData(data);
		return data;
	}

	/**
	 * Creates a new {@link DefaultData} instance matching this .NET data object.
	 *
	 * @return a new {@link DefaultData} instance
	 */
	protected abstract MethodSensorData toNewDefaultData();

	/**
	 * Returns if this object can be converted into an {@link InvocationSequenceData}.
	 *
	 * @return if {@link DotNetMethodSensorData#toInvocationSequenceData()} is usable
	 */
	public abstract boolean isInvocationSequenceConvertible();

	/**
	 * Creates an {@link InvocationSequenceData} based on the represented data.<br>
	 * <b>Note:</b> First check if this method is usable by calling
	 * {@link DotNetMethodSensorData#isInvocationSequenceConvertible()}!
	 *
	 * @return an {@link InvocationSequenceData}
	 */
	public InvocationSequenceData toInvocationSequenceData() {
		if (isInvocationSequenceConvertible()) {
			InvocationSequenceData invocData = new InvocationSequenceData();
			fillInvocationSequeneData(invocData);
			return invocData;
		} else {
			return null;
		}
	}

	/**
	 * Fills the passed an {@link InvocationSequenceData} with attributes. Should be overridden by
	 * subclasses adding their attributes and calling {@code super.fillInvocationSequeneData(...)}.
	 *
	 * @param invocData
	 *            the an {@link InvocationSequenceData} to be filled
	 */
	protected void fillInvocationSequeneData(InvocationSequenceData invocData) {
		if (invocData == null) {
			throw new IllegalArgumentException("DefaultData must not be null");
		}

		invocData.setTimeStamp(new Timestamp(timestamp));
		invocData.setPlatformIdent(platformId);
		invocData.setSensorTypeIdent(methodSensorId);
		invocData.setMethodIdent(methodId);
	}

	/**
	 * Adds all attributes to the passed {@link DefaultData}. Should be overridden by subclasses
	 * adding their attributes and calling {@code super.addAttributesToDefaultData(...)}.
	 *
	 * @param defaultData
	 *            the DefaultData to add the attributes to
	 */
	protected void addAttributesToDefaultData(MethodSensorData defaultData) {
		if (defaultData == null) {
			throw new IllegalArgumentException("DefaultData must not be null");
		}

		defaultData.setTimeStamp(new Timestamp(timestamp));
		defaultData.setPlatformIdent(platformId);
		defaultData.setSensorTypeIdent(methodSensorId);
		defaultData.setMethodIdent(methodId);
	}

}
