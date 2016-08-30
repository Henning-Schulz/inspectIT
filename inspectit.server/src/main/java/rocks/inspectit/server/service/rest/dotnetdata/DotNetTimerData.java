/**
 *
 */
package rocks.inspectit.server.service.rest.dotnetdata;

import rocks.inspectit.shared.all.communication.MethodSensorData;
import rocks.inspectit.shared.all.communication.data.SingleTimerData;

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
		return new SingleTimerData();
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	protected void addAttributesToDefaultData(MethodSensorData defaultData) {
		if (!(defaultData instanceof SingleTimerData)) {
			throw new IllegalArgumentException("Requires SingleTimerData, but was " + defaultData.getClass().getName());
		}

		SingleTimerData timerData = (SingleTimerData) defaultData;

		super.addAttributesToDefaultData(timerData);

		timerData.setThreadId(threadId);
		timerData.setStartTime(startTime);
		timerData.setEndTime(endTime);
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public boolean isInvocationSequenceConvertible() {
		return false;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public String toString() {
		return "Method " + getMethodId() + " on thread " + threadId + ": " + startTime + " - " + endTime + " (" + (endTime - startTime) + " ns)";
	}

}
