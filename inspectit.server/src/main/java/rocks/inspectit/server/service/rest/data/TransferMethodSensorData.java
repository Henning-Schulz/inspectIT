package rocks.inspectit.server.service.rest.data;

import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import rocks.inspectit.shared.all.communication.DefaultData;
import rocks.inspectit.shared.all.communication.MethodSensorData;
import rocks.inspectit.shared.all.communication.data.ParameterContentData;
import rocks.inspectit.shared.all.communication.data.ParameterContentType;

/**
 * @author Henning Schulz
 *
 */
public abstract class TransferMethodSensorData extends TransferData {

	private static final String KEY_RETURN = "return";

	private long methodIdent;

	private Map<String, String> parameterContentData;

	/**
	 * Gets {@link #methodIdent}.
	 *
	 * @return {@link #methodIdent}
	 */
	public long getMethodIdent() {
		return this.methodIdent;
	}

	/**
	 * Sets {@link #methodIdent}.
	 *
	 * @param methodIdent
	 *            New value for {@link #methodIdent}
	 */
	public void setMethodIdent(long methodIdent) {
		this.methodIdent = methodIdent;
	}

	/**
	 * Gets {@link #parameterContentData}.
	 *
	 * @return {@link #parameterContentData}
	 */
	public Map<String, String> getParameterContentData() {
		return this.parameterContentData;
	}

	/**
	 * Sets {@link #parameterContentData}.
	 *
	 * @param parameterContentData
	 *            New value for {@link #parameterContentData}
	 */
	public void setParameterContentData(Map<String, String> parameterContentData) {
		this.parameterContentData = parameterContentData;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	protected DefaultData toCorrespondingDefaultData() {
		MethodSensorData data = toMethodSensorData();

		if (null != data) {
			data.setMethodIdent(methodIdent);
			data.setParameterContentData(convertParameterContents());
		}

		return data;
	}

	protected abstract MethodSensorData toMethodSensorData();

	private Set<ParameterContentData> convertParameterContents() {
		Set<ParameterContentData> contents = new HashSet<>();

		for (Map.Entry<String, String> contentEntry : parameterContentData.entrySet()) {
			contents.add(convertParamEntry(contentEntry));
		}

		return contents;
	}

	private ParameterContentData convertParamEntry(Map.Entry<String, String> contentEntry) {
		assert contentEntry.getKey() != null;
		assert contentEntry.getValue() != null;

		String key = contentEntry.getKey();
		ParameterContentData contentData = new ParameterContentData();
		contentData.setContent(contentEntry.getValue());

		if (KEY_RETURN.equals(key)) {
			contentData.setContentType(ParameterContentType.RETURN);
		} else if (key.matches("[0-9]+")) {
			int position = Integer.parseInt(key);
			contentData.setContentType(ParameterContentType.PARAM);
			contentData.setSignaturePosition(position);
		} else {
			contentData.setContentType(ParameterContentType.FIELD);
			contentData.setName(key);
		}

		return contentData;
	}

}
