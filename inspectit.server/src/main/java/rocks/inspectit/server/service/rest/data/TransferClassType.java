package rocks.inspectit.server.service.rest.data;

import java.util.Set;

import rocks.inspectit.shared.all.instrumentation.classcache.ClassType;
import rocks.inspectit.shared.all.instrumentation.classcache.InterfaceType;
import rocks.inspectit.shared.all.instrumentation.classcache.Type;

/**
 * @author Henning Schulz
 *
 */
public class TransferClassType extends TransferType {

	private String[] superClasses = null;

	private String[] realizedInterfaces = null;

	/**
	 * A list of all methods of this interface.
	 */
	private Set<TransferMethodType> methods = null;

	/**
	 * @param fqn
	 * @param hash
	 * @param modifiers
	 */
	public TransferClassType(String fqn, String hash, int modifiers) {
		super(fqn, hash, modifiers);
	}

	/**
	 *
	 */
	public TransferClassType() {
	}

	/**
	 * Gets {@link #superClasses}.
	 *
	 * @return {@link #superClasses}
	 */
	public String[] getSuperClasses() {
		return this.superClasses;
	}

	/**
	 * Sets {@link #superClasses}.
	 *
	 * @param superClasses
	 *            New value for {@link #superClasses}
	 */
	public void setSuperClasses(String[] superClasses) {
		this.superClasses = superClasses;
	}

	/**
	 * Gets {@link #realizedInterfaces}.
	 *
	 * @return {@link #realizedInterfaces}
	 */
	public String[] getRealizedInterfaces() {
		return this.realizedInterfaces;
	}

	/**
	 * Sets {@link #realizedInterfaces}.
	 *
	 * @param realizedInterfaces
	 *            New value for {@link #realizedInterfaces}
	 */
	public void setRealizedInterfaces(String[] realizedInterfaces) {
		this.realizedInterfaces = realizedInterfaces;
	}

	/**
	 * Gets {@link #methods}.
	 *
	 * @return {@link #methods}
	 */
	public Set<TransferMethodType> getMethods() {
		return this.methods;
	}

	/**
	 * Sets {@link #methods}.
	 *
	 * @param methods
	 *            New value for {@link #methods}
	 */
	public void setMethods(Set<TransferMethodType> methods) {
		this.methods = methods;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public Type toType() {
		ClassType type = new ClassType(getFqn(), getHash(), getModifiers());

		if (null != superClasses) {
			for (String sup : superClasses) {
				type.addSuperClass(new ClassType(sup));
			}
		}

		if (null != realizedInterfaces) {
			for (String interf : realizedInterfaces) {
				type.addInterface(new InterfaceType(interf));
			}
		}

		if (methods != null) {
			for (TransferMethodType meth : methods) {
				type.addMethod(meth.toType());
			}
		}

		return type;
	}

}
