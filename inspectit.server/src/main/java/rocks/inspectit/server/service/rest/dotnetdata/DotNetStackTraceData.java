/**
 *
 */
package rocks.inspectit.server.service.rest.dotnetdata;

import java.sql.Timestamp;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import rocks.inspectit.shared.all.communication.MethodSensorData;
import rocks.inspectit.shared.all.communication.data.InvocationSequenceData;
import rocks.inspectit.shared.all.communication.data.StackTraceData;
import rocks.inspectit.shared.all.communication.data.StackTraceSample;
import rocks.inspectit.shared.all.communication.data.TimerData;

/**
 * @author Henning Schulz
 *
 */
public class DotNetStackTraceData extends DotNetMethodSensorData {

	private long threadId;

	private long baseMethodId;
	private long startTime;
	private long endTime;

	private DotNetStackTraceSample[] traces;

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
	public DotNetStackTraceSample[] getTraces() {
		return traces;
	}

	/**
	 * Sets {@link #traces}.
	 *
	 * @param traces
	 *            New value for {@link #traces}
	 */
	public void setTraces(DotNetStackTraceSample[] traces) {
		this.traces = traces;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	protected MethodSensorData toNewDefaultData() {
		return new StackTraceData();
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	protected void addAttributesToDefaultData(MethodSensorData defaultData) {
		if (!(defaultData instanceof StackTraceData)) {
			throw new IllegalArgumentException("Requires StackTraceData, but was " + defaultData.getClass().getName());
		}

		super.addAttributesToDefaultData(defaultData);
		StackTraceData stackTraceData = (StackTraceData) defaultData;
		StackTraceSample[] traces = new StackTraceSample[getTraces().length];

		int i = 0;
		for (DotNetStackTraceSample dotNetSample : getTraces()) {
			traces[i] = dotNetSample.toDefaultDataSample();
			i++;
		}

		stackTraceData.setThreadId(getThreadId());
		stackTraceData.setBaseMethodId(getBaseMethodId());
		stackTraceData.setStartTime(getStartTime());
		stackTraceData.setEndTime(getEndTime());
		stackTraceData.setTraces(traces);
	}

	private double getAsStartTime(int index) {
		assert index >= 0 && index < traces.length;

		double actual = traces[index].getTimestamp();

		if (index > 0) {
			double before = traces[index - 1].getTimestamp();
			return (actual + before) / 2;
		} else if (index < traces.length - 1) {
			double after = traces[index + 1].getTimestamp();
			return actual - (after - actual) / 2;
		} else {
			return actual;
		}
	}

	private double getAsEndTime(int index) {
		assert index >= 0 && index < traces.length;

		double actual = traces[index].getTimestamp();

		if (index < traces.length - 1) {
			double after = traces[index + 1].getTimestamp();
			return (actual + after) / 2;
		} else if (index > 0) {
			double before = traces[index - 1].getTimestamp();
			return actual + (actual - before) / 2;
		} else {
			return actual;
		}
	}

	/**
	 * Adds all methods of the traces at the specified depth within start (inclusive) and end
	 * (exclusive) to the specified InvocationSequenceData.
	 *
	 * @param parent
	 * @param depth
	 * @param start
	 * @param end
	 */
	private long addNestedSequences(InvocationSequenceData parent, int depth, int start, int end) {
		long childCount = 0;
		// TODO: Update childCount

		boolean hasNested = false;
		List<InvocationSequenceData> nestedSequences = new ArrayList<>();
		int positionInParent = 0;
		InvocationSequenceData currData = null;
		long currMethod = -1;
		long timestamp = -1;
		double startTime = -1;
		double endTime = -1;
		int currStart = 0;

		for (int i = start; i < end; i++) {
			DotNetStackTraceSample trace = traces[i];

			if (depth >= trace.size() || (trace.getOffset() <= depth && currMethod != trace.at(depth))) {
				// stack ended or new method

				if (currData != null) {
					currData.setEnd(endTime);
					currData.setDuration(endTime - startTime);

					if (hasNested) {
						childCount += addNestedSequences(currData, depth + 1, currStart, i);
					}

					positionInParent++;
				}

				if (depth >= trace.size()) {
					// stack ended

					currMethod = -1;
					startTime = -1;
					timestamp = -1;
					currData = null;
				} else {
					// new method

					currMethod = trace.at(depth);
					startTime = getAsStartTime(i);
					timestamp = trace.getTimestamp();
					currData = new InvocationSequenceData(new Timestamp(timestamp), getPlatformId(), getMethodSensorId(), currMethod);
					currData.setParentSequence(parent);
					currData.setStart(startTime);
					currData.setPosition(positionInParent);
					nestedSequences.add(currData);
					currStart = i;
					childCount++;
				}

				hasNested = false;
			}

			endTime = getAsEndTime(i);

			if (trace.size() > depth + 1) {
				hasNested = true;
			}
		}

		if (currData != null) {
			currData.setEnd(endTime);
			currData.setDuration(endTime - startTime);

			if (hasNested) {
				childCount += addNestedSequences(currData, depth + 1, currStart, end);
			}
		}

		parent.setNestedSequences(nestedSequences);
		parent.setChildCount(childCount);

		return childCount;
	}

	private void addTimerData(InvocationSequenceData data) {
		// TODO SensorTypeId?
		TimerData timerData = new TimerData(data.getTimeStamp(), data.getPlatformIdent(), data.getSensorTypeIdent(), data.getMethodIdent());

		double time = data.getDuration();
		timerData.increaseCount();
		timerData.addDuration(time);

		timerData.calculateMax(time);
		timerData.calculateMin(time);

		data.setTimerData(timerData);

		for (InvocationSequenceData child : data.getNestedSequences()) {
			addTimerData(child);
		}
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public boolean isInvocationSequenceConvertible() {
		return true;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	protected void fillInvocationSequeneData(InvocationSequenceData invocData) {
		super.fillInvocationSequeneData(invocData);

		if (traces[0].size() == 0) {
			invocData.setMethodIdent(-1);
			return;
		}

		double start = getAsStartTime(0);
		double end = getAsEndTime(traces.length - 1);
		invocData.setMethodIdent(traces[0].at(0));
		invocData.setStart(start);
		invocData.setEnd(end);
		invocData.setDuration(end - start);

		addNestedSequences(invocData, 1, 0, traces.length);
		addTimerData(invocData);
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public String toString() {
		StringBuilder builder = new StringBuilder();
		DecimalFormat format = new DecimalFormat("00000");
		SimpleDateFormat dateFormat = new SimpleDateFormat("dd.MM.yyyy, HH:mm:ss.SSS");

		builder.append("Stack traces for platform ");
		builder.append(getPlatformId());
		builder.append(" and thread ");
		builder.append(threadId);
		builder.append("\n");

		builder.append("Base method is ");
		builder.append(baseMethodId);
		builder.append(", duration from ");
		builder.append(dateFormat.format(new Date(startTime / 1000000)));
		builder.append(" to ");
		builder.append(dateFormat.format(new Date(endTime / 1000000)));
		builder.append(". Overall duration is ");
		builder.append((endTime - startTime) / 1000000);
		builder.append(" ms.\n");

		builder.append("Timestamps: ");
		long startTime = traces[0].getTimestamp();

		for (DotNetStackTraceSample trace : traces) {
			builder.append(" ");
			builder.append(format.format((trace.getTimestamp() - startTime) / 1000000));
			builder.append(" ");
		}

		builder.append("\nMethods:    ");

		boolean stop = false;
		int depth = 0;

		while (!stop) {
			stop = true;

			for (DotNetStackTraceSample trace : traces) {
				builder.append(" ");
				if (depth < trace.getOffset()) {
					builder.append(" ... ");
				} else if (depth >= trace.size()) {
					builder.append("     ");
				} else {
					builder.append(format.format(trace.at(depth)));
				}

				if (trace.size() > depth + 1) {
					stop = false;
				}

				builder.append(" ");
			}

			builder.append("\n            ");

			depth++;
		}

		return builder.toString();
	}

}
