package rocks.inspectit.server.service.rest.data;

import rocks.inspectit.shared.all.instrumentation.classcache.InterfaceType;
import rocks.inspectit.shared.all.instrumentation.classcache.Type;

/**
 * @author Henning Schulz
 *
 */
public class TransferInterfaceType extends TransferType {

	private String[] superInterfaces;

	/**
	 * @param fqn
	 * @param hash
	 * @param modifiers
	 */
	public TransferInterfaceType(String fqn, String hash, int modifiers) {
		super(fqn, hash, modifiers);
	}

	/**
	 *
	 */
	public TransferInterfaceType() {
	}

	/**
	 * Gets {@link #superInterfaces}.
	 *
	 * @return {@link #superInterfaces}
	 */
	public String[] getSuperInterfaces() {
		return this.superInterfaces;
	}

	/**
	 * Sets {@link #superInterfaces}.
	 *
	 * @param superInterfaces
	 *            New value for {@link #superInterfaces}
	 */
	public void setSuperInterfaces(String[] superInterfaces) {
		this.superInterfaces = superInterfaces;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public Type toType() {
		InterfaceType type = new InterfaceType(getFqn(), getHash(), getModifiers());

		for (String interf : superInterfaces) {
			type.addSuperInterface(new InterfaceType(interf));
		}

		return type;
	}

}
