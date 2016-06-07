/**
 *
 */
package rocks.inspectit.server.service.rest.dotnetdata;

import rocks.inspectit.shared.all.communication.MethodSensorData;

/**
 * @author Henning Schulz
 *
 */
public class DotNetHelloData extends DotNetMethodSensorData {

	private String greeting;

	/**
	 * Sets {@link #greeting}.
	 *
	 * @param greeting
	 *            New value for {@link #greeting}
	 */
	public void setGreeting(String greeting) {
		this.greeting = greeting;
	}

	/**
	 * Gets {@link #greeting}.
	 *
	 * @return {@link #greeting}
	 */
	public String getGreeting() {
		return greeting;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public String toString() {
		return greeting;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	protected MethodSensorData toNewDefaultData() {
		// No matching MethodSensorData available
		return null;
	}

}
