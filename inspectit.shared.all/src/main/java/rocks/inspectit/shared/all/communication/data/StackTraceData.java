/**
 *
 */
package rocks.inspectit.shared.all.communication.data;

import rocks.inspectit.shared.all.communication.MethodSensorData;

/**
 * @author Henning Schulz
 *
 */
public class StackTraceData extends MethodSensorData {

	/**
	 * Generated serial UID.
	 */
	private static final long serialVersionUID = -2288816684899047450L;

	private long threadId;

	private long baseMethodId;
	private long startTime;
	private long endTime;

	private StackTraceSample[] traces;

	/**
	 * Gets {@link #threadId}.
	 *
	 * @return {@link #threadId}
	 */
	public long getThreadId() {
		return threadId;
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
	 * Gets {@link #baseMethodId}.
	 *
	 * @return {@link #baseMethodId}
	 */
	public long getBaseMethodId() {
		return baseMethodId;
	}

	/**
	 * Sets {@link #baseMethodId}.
	 *
	 * @param baseMethodId
	 *            New value for {@link #baseMethodId}
	 */
	public void setBaseMethodId(long baseMethodId) {
		this.baseMethodId = baseMethodId;
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

	/**
	 * Gets {@link #traces}.
	 *
	 * @return {@link #traces}
	 */
	public StackTraceSample[] getTraces() {
		return traces;
	}

	/**
	 * Sets {@link #traces}.
	 *
	 * @param traces
	 *            New value for {@link #traces}
	 */
	public void setTraces(StackTraceSample[] traces) {
		this.traces = traces;
	}

}
