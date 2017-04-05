package rocks.inspectit.server.service.rest.data;

import rocks.inspectit.shared.all.instrumentation.classcache.AnnotationType;
import rocks.inspectit.shared.all.instrumentation.classcache.Type;

/**
 * @author Henning Schulz
 *
 */
public class TransferAnnotationType extends TransferType {

	/**
	 * @param fqn
	 * @param hash
	 * @param modifiers
	 */
	public TransferAnnotationType(String fqn, String hash, int modifiers) {
		super(fqn, hash, modifiers);
	}

	/**
	 *
	 */
	public TransferAnnotationType() {
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public Type toType() {
		return new AnnotationType(getFqn(), getHash(), getModifiers());
	}

}
