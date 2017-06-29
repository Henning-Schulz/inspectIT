package rocks.inspectit.server.service.rest.data;

import rocks.inspectit.shared.all.communication.MethodSensorData;
import rocks.inspectit.shared.all.communication.data.TimerData;

/**
 * @author Henning Schulz
 *
 */
public class TransferTimerSensorData extends TransferMethodSensorData {

	private long threadId;

	private long startTime;

	private long endTime;

	/**
	 * Gets {@link #threadId}.
	 *
	 * @return {@link #threadId}
	 */
	public long getThreadId() {
		return this.threadId;
	}

	/**
	 * Sets {@link #threadId}.
	 *
	 * @param threadId
	 *            New value for {@link #threadId}
	 */
	public void setThreadId(long threadId) {
		this.threadId = threadId;
	}

	/**
	 * Gets {@link #startTime}.
	 *
	 * @return {@link #startTime}
	 */
	public long getStartTime() {
		return this.startTime;
	}

	/**
	 * Sets {@link #startTime}.
	 *
	 * @param startTime
	 *            New value for {@link #startTime}
	 */
	public void setStartTime(long startTime) {
		this.startTime = startTime;
	}

	/**
	 * Gets {@link #endTime}.
	 *
	 * @return {@link #endTime}
	 */
	public long getEndTime() {
		return this.endTime;
	}

	/**
	 * Sets {@link #endTime}.
	 *
	 * @param endTime
	 *            New value for {@link #endTime}
	 */
	public void setEndTime(long endTime) {
		this.endTime = endTime;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	protected MethodSensorData toMethodSensorData() {
		double duration = (endTime - startTime) / 1000000;

		TimerData data = new TimerData();
		data.setDuration(duration);
		data.setCount(1);
		return data;
	}

}
