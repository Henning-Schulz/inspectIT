package rocks.inspectit.server.service.rest.data;

import java.sql.Timestamp;

import org.codehaus.jackson.annotate.JsonSubTypes;
import org.codehaus.jackson.annotate.JsonTypeInfo;

import rocks.inspectit.shared.all.communication.DefaultData;

/**
 * @author Henning Schulz
 *
 */
@JsonTypeInfo(use = JsonTypeInfo.Id.NAME, include = JsonTypeInfo.As.PROPERTY, property = "dataType")
@JsonSubTypes({ @JsonSubTypes.Type(name = "TimerData", value = TransferTimerSensorData.class) })
public abstract class TransferData {

	private long timestampMs;

	/**
	 * Gets {@link #timestampMs}.
	 *
	 * @return {@link #timestampMs}
	 */
	public long getTimestampMs() {
		return this.timestampMs;
	}

	/**
	 * Sets {@link #timestampMs}.
	 *
	 * @param timestampMs
	 *            New value for {@link #timestampMs}
	 */
	public void setTimestampMs(long timestampMs) {
		this.timestampMs = timestampMs;
	}

	public DefaultData toDefaultData(long platformIdent, long sensorTypeIdent) {
		DefaultData data = toCorrespondingDefaultData();

		if (null != data) {
			data.setPlatformIdent(platformIdent);
			data.setSensorTypeIdent(sensorTypeIdent);
			data.setTimeStamp(new Timestamp(timestampMs));
		}

		return data;
	}

	protected abstract DefaultData toCorrespondingDefaultData();

}
