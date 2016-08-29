/**
 *
 */
package rocks.inspectit.server.service.rest.dotnetdata;

import rocks.inspectit.shared.all.communication.MethodSensorData;
import rocks.inspectit.shared.all.communication.data.TimerData;

/**
 * @author Henning Schulz
 *
 */
public class DotNetTimerData extends DotNetMethodSensorData {

	private int threadId;
	private long startTime;
	private long endTime;

	/**
	 * Sets {@link #threadId}.
	 *
	 * @param threadId
	 *            New value for {@link #threadId}
	 */
	public void setThreadId(int threadId) {
		this.threadId = threadId;
	}

	/**
	 * Gets {@link #threadId}.
	 *
	 * @return {@link #threadId}
	 */
	public int getThreadId() {
		return threadId;
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
	 * Gets {@link #startTime}.
	 *
	 * @return {@link #startTime}
	 */
	public long getStartTime() {
		return startTime;
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
	 * Gets {@link #endTime}.
	 *
	 * @return {@link #endTime}
	 */
	public long getEndTime() {
		return endTime;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	protected MethodSensorData toNewDefaultData() {
		return new TimerData();
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	protected void addAttributesToDefaultData(MethodSensorData defaultData) {
		if (!(defaultData instanceof TimerData)) {
			throw new IllegalArgumentException("Requires TimerData, but was " + defaultData.getClass().getName());
		}

		TimerData timerData = (TimerData) defaultData;

		super.addAttributesToDefaultData(timerData);

		timerData.set
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public boolean isInvocationSequenceConvertible() {
		return false;
	}

}
