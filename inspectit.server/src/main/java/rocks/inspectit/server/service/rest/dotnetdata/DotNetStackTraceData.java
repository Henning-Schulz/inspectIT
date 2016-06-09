/**
 *
 */
package rocks.inspectit.server.service.rest.dotnetdata;

import java.text.DecimalFormat;

import rocks.inspectit.shared.all.communication.MethodSensorData;

/**
 * @author Henning Schulz
 *
 */
public class DotNetStackTraceData extends DotNetMethodSensorData {

	private long threadId;

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
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public String toString() {
		StringBuilder builder = new StringBuilder();

		builder.append("Stack traces for thread ");
		builder.append(threadId);
		builder.append("\n");

		boolean stop = false;
		int depth = 0;
		DecimalFormat format = new DecimalFormat("00000");

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

					if (trace.size() > depth + 1) {
						stop = false;
					}
				}
				builder.append(" ");
			}

			builder.append("\n");

			depth++;
		}

		return builder.toString();
	}

}
