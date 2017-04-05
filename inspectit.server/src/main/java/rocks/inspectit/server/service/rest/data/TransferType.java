package rocks.inspectit.server.service.rest.data;

import org.codehaus.jackson.annotate.JsonSubTypes;
import org.codehaus.jackson.annotate.JsonTypeInfo;

import rocks.inspectit.shared.all.instrumentation.classcache.Type;

/**
 * @author Henning Schulz
 *
 */
@JsonTypeInfo(use = JsonTypeInfo.Id.NAME, include = JsonTypeInfo.As.PROPERTY, property = "type")
@JsonSubTypes({ @JsonSubTypes.Type(name = "Class", value = TransferClassType.class), @JsonSubTypes.Type(name = "Interface", value = TransferInterfaceType.class),
	@JsonSubTypes.Type(name = "Annotation", value = TransferAnnotationType.class) })
public abstract class TransferType {

	private String fqn;
	private String hash;
	private int modifiers;

	/**
	 * @param fqn
	 * @param hash
	 * @param modifiers
	 */
	public TransferType(String fqn, String hash, int modifiers) {
		this.fqn = fqn;
		this.hash = hash;
		this.modifiers = modifiers;
	}

	/**
	 *
	 */
	public TransferType() {
	}

	/**
	 * Gets {@link #fqn}.
	 *
	 * @return {@link #fqn}
	 */
	public String getFqn() {
		return this.fqn;
	}

	/**
	 * Sets {@link #fqn}.
	 *
	 * @param fqn
	 *            New value for {@link #fqn}
	 */
	public void setFqn(String fqn) {
		this.fqn = fqn;
	}

	/**
	 * Gets {@link #hash}.
	 *
	 * @return {@link #hash}
	 */
	public String getHash() {
		return this.hash;
	}

	/**
	 * Sets {@link #hash}.
	 *
	 * @param hash
	 *            New value for {@link #hash}
	 */
	public void setHash(String hash) {
		this.hash = hash;
	}

	/**
	 * Gets {@link #modifiers}.
	 *
	 * @return {@link #modifiers}
	 */
	public int getModifiers() {
		return this.modifiers;
	}

	/**
	 * Sets {@link #modifiers}.
	 *
	 * @param modifiers
	 *            New value for {@link #modifiers}
	 */
	public void setModifiers(int modifiers) {
		this.modifiers = modifiers;
	}

	public abstract Type toType();

}
