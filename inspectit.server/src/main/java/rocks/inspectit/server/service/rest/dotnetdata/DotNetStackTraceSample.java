/**
 *
 */
package rocks.inspectit.server.service.rest.dotnetdata;

/**
 * @author Henning Schulz
 *
 */
public class DotNetStackTraceSample {

	private long[] trace;
	private int offset;
	private long timestamp;

	/**
	 * Returns the method at the specified depth. Note that depth 0 means to retrieve the top method
	 * that is stored at the and of the trace array.
	 *
	 * @param depth
	 * @return
	 */
	public long at(int depth) {
		if (depth < offset) {
			throw new IllegalArgumentException("No information on depth " + depth + " stored. Offset is " + offset);
		} else if (depth >= size()) {
			throw new IllegalArgumentException("Requested trace at depth " + depth + ". Size is only " + size());
		}

		return trace[trace.length - 1 + offset - depth];
	}

	public int size() {
		return offset + trace.length;
	}

	/**
	 * Gets {@link #trace}.
	 *
	 * @return {@link #trace}
	 */
	public long[] getTrace() {
		return trace;
	}

	/**
	 * Sets {@link #trace}.
	 *
	 * @param trace
	 *            New value for {@link #trace}
	 */
	public void setTrace(long[] trace) {
		if (trace == null) {
			this.trace = new long[0];
		} else {
			this.trace = trace;
		}
	}

	/**
	 * Gets {@link #offset}.
	 *
	 * @return {@link #offset}
	 */
	public int getOffset() {
		return offset;
	}

	/**
	 * Sets {@link #offset}.
	 *
	 * @param offset
	 *            New value for {@link #offset}
	 */
	public void setOffset(int offset) {
		this.offset = offset;
	}

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
	 * {@inheritDoc}
	 */
	@Override
	public String toString() {
		StringBuilder builder = new StringBuilder();

		boolean first = true;
		int i = 0;

		while (i < getOffset()) {
			if (first) {
				first = false;
			} else {
				builder.append("->");
			}

			builder.append("...");
			i++;
		}

		while (i < size()) {
			if (first) {
				first = false;
			} else {
				builder.append("->");
			}

			builder.append(at(i));
			i++;
		}

		return builder.toString();
	}

}
