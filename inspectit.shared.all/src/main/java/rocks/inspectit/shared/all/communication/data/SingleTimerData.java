/**
 *
 */
package rocks.inspectit.shared.all.communication.data;

import rocks.inspectit.shared.all.communication.MethodSensorData;

/**
 * @author Henning Schulz
 *
 */
public class SingleTimerData extends MethodSensorData {

	/**
	 *
	 */
	private static final long serialVersionUID = 440614299566046922L;

	private int threadId;
	private long startTime;
	private long endTime;

	/**
	 * Gets {@link #threadId}.
	 * 
	 * @return {@link #threadId}
	 */
	public int getThreadId() {
		return threadId;
	}

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
	 * Gets {@link #startTime}.
	 * 
	 * @return {@link #startTime}
	 */
	public long getStartTime() {
		return startTime;
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
		return endTime;
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

}
