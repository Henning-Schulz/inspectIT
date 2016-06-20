package rocks.inspectit.ui.rcp.ci.form.part;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;

import org.eclipse.jface.fieldassist.ControlDecoration;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Layout;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.IEditorPart;
import org.eclipse.ui.IPropertyListener;
import org.eclipse.ui.forms.IFormColors;
import org.eclipse.ui.forms.IMessageManager;
import org.eclipse.ui.forms.SectionPart;
import org.eclipse.ui.forms.editor.FormPage;
import org.eclipse.ui.forms.widgets.FormText;
import org.eclipse.ui.forms.widgets.FormToolkit;
import org.eclipse.ui.forms.widgets.Section;

import rocks.inspectit.shared.cs.ci.Environment;
import rocks.inspectit.shared.cs.ci.sensor.StringConstraintSensorConfig;
import rocks.inspectit.shared.cs.ci.sensor.dotNet.DotNetStackTraceSensorConfig;
import rocks.inspectit.shared.cs.ci.sensor.dotNet.DotNetStackTraceSensorConfig.StackTraceProviderType;
import rocks.inspectit.shared.cs.ci.sensor.dotNet.DotNetStackTraceSensorConfig.TriggerType;
import rocks.inspectit.shared.cs.ci.sensor.exception.impl.ExceptionSensorConfig;
import rocks.inspectit.shared.cs.ci.sensor.method.IMethodSensorConfig;
import rocks.inspectit.shared.cs.ci.sensor.method.impl.HttpSensorConfig;
import rocks.inspectit.shared.cs.ci.sensor.method.impl.InvocationSequenceSensorConfig;
import rocks.inspectit.shared.cs.ci.sensor.method.impl.PreparedStatementSensorConfig;
import rocks.inspectit.shared.cs.ci.sensor.method.impl.StatementSensorConfig;
import rocks.inspectit.shared.cs.ci.sensor.method.impl.TimerSensorConfig;
import rocks.inspectit.ui.rcp.InspectIT;
import rocks.inspectit.ui.rcp.InspectITImages;
import rocks.inspectit.ui.rcp.ci.form.input.EnvironmentEditorInput;
import rocks.inspectit.ui.rcp.formatter.ImageFormatter;
import rocks.inspectit.ui.rcp.formatter.TextFormatter;
import rocks.inspectit.ui.rcp.validation.ValidationControlDecoration;

/**
 * Part for displaying sensor options.
 *
 * @author Ivan Senic
 *
 */
public class SensorOptionsPart extends SectionPart implements IPropertyListener {

	/**
	 * Environment to be edited.
	 */
	private Environment environment;

	/**
	 * Exception sensor simple option.
	 */
	private Button exceptionSimple;

	/**
	 * Exception sensor enhanced option.
	 */
	private Button exceptionEnhanced;

	/**
	 * All {@link StringConstraintComponent} on the page.
	 */
	private final List<StringConstraintComponent> constrainComponents = new ArrayList<>();

	/**
	 * Form page part belongs to.
	 */
	private final FormPage formPage;

	/**
	 * Default constructor.
	 *
	 * @param formPage
	 *            {@link FormPage} section belongs to.
	 * @param parent
	 *            Parent composite.
	 * @param toolkit
	 *            {@link FormToolkit}
	 * @param style
	 *            Style used for creating the section.
	 */
	public SensorOptionsPart(FormPage formPage, Composite parent, FormToolkit toolkit, int style) {
		super(parent, toolkit, style);
		EnvironmentEditorInput input = (EnvironmentEditorInput) formPage.getEditor().getEditorInput();
		this.environment = input.getEnvironment();
		this.formPage = formPage;
		this.formPage.getEditor().addPropertyListener(this);

		// client
		createClient(getSection(), toolkit);

		// text and description on our own
		getSection().setText("Sensor Options");
		Label label = toolkit.createLabel(getSection(), "Define options for specific sensors");
		label.setForeground(toolkit.getColors().getColor(IFormColors.TITLE));
		getSection().setDescriptionControl(label);
	}

	/**
	 * Creates complete client.
	 *
	 * @param section
	 *            {@link Section}
	 * @param toolkit
	 *            {@link FormToolkit}
	 */
	private void createClient(Section section, FormToolkit toolkit) {
		Composite mainComposite = toolkit.createComposite(section);
		int layoutColumns = 4;
		GridLayout gridLayout = new GridLayout(layoutColumns, false);
		gridLayout.horizontalSpacing = 10;
		mainComposite.setLayout(gridLayout);
		section.setClient(mainComposite);

		IMessageManager messageManager = formPage.getManagedForm().getMessageManager();
		// method sensors
		for (IMethodSensorConfig methodSensorConfig : environment.getMethodSensorConfigs()) {
			if (methodSensorConfig instanceof DotNetStackTraceSensorConfig) {
				DotNetStackTraceComponent dotNetComponent = new DotNetStackTraceComponent((DotNetStackTraceSensorConfig) methodSensorConfig, messageManager);
				constrainComponents.add(dotNetComponent);
				dotNetComponent.createComponent(mainComposite, toolkit, "String length of captured context(s):",
						"Defines the maximum string length of captured context (parameters, fields, return values) for timer sensor.", layoutColumns);

			} else if (methodSensorConfig instanceof StringConstraintSensorConfig) {
				StringConstraintComponent stringConstraintComponent = new StringConstraintComponent((StringConstraintSensorConfig) methodSensorConfig, messageManager);
				constrainComponents.add(stringConstraintComponent);

				if (methodSensorConfig instanceof TimerSensorConfig) {
					stringConstraintComponent.createComponent(mainComposite, toolkit, "String length of captured context(s):",
							"Defines the maximum string length of captured context (parameters, fields, return values) for timer sensor.", layoutColumns);
				} else if (methodSensorConfig instanceof HttpSensorConfig) {
					stringConstraintComponent.createComponent(mainComposite, toolkit, "String length of captured HTTP data:",
							"Defines the maximum string length of captured HTTP data (parameters, headers, attributes, etc) for HTTP sensor.", layoutColumns);
				} else if (methodSensorConfig instanceof InvocationSequenceSensorConfig) {
					stringConstraintComponent.createComponent(mainComposite, toolkit, "String length of captured context(s):",
							"Defines the maximum string length of captured context (parameters, fields, return values) for invocation sensor.", layoutColumns);
				} else if (methodSensorConfig instanceof PreparedStatementSensorConfig) {
					stringConstraintComponent.createComponent(mainComposite, toolkit, "String length of SQLs:",
							"Defines the maximum string length of SQL strings and parameters for prepared statement sensor.", layoutColumns);
				} else if (methodSensorConfig instanceof StatementSensorConfig) {
					stringConstraintComponent.createComponent(mainComposite, toolkit, "String length of SQLs:", "Defines the maximum string length of SQL strings and parameters for statement sensor.",
							layoutColumns);
				}
			}
		}

		// exception sensor
		ExceptionSensorConfig exceptionSensorConfig = (ExceptionSensorConfig) environment.getExceptionSensorConfig();
		StringConstraintComponent stringConstraintComponent = new StringConstraintComponent(exceptionSensorConfig, messageManager);
		constrainComponents.add(stringConstraintComponent);
		stringConstraintComponent.createComponent(mainComposite, toolkit, "String length of stack trace & messages:",
				"Defines the maximum string length of captured stack traces and exception messages for exception sensor.", layoutColumns);

		toolkit.createLabel(mainComposite, "Mode:").setLayoutData(getIndentGridData());
		Composite radioGroupHelp = toolkit.createComposite(mainComposite);
		radioGroupHelp.setLayout(getLayoutForRadioGroup());
		radioGroupHelp.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));
		exceptionSimple = toolkit.createButton(radioGroupHelp, "Simple", SWT.RADIO);
		exceptionSimple.setSelection(true);
		exceptionSimple.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));
		exceptionEnhanced = toolkit.createButton(radioGroupHelp, "Enhanced", SWT.RADIO);
		toolkit.createLabel(mainComposite, "");
		createInfoLabel(mainComposite, toolkit,
				"Simple mode will only provide the information where have the exceptions been created. All other informations like passed and handled will be available when enhanced mode if on. But, with simple mode there will be no performance overhead for using the exception sensor.");

		// set selection for simple / enhanced
		exceptionSimple.setSelection(!exceptionSensorConfig.isEnhanced());
		exceptionEnhanced.setSelection(exceptionSensorConfig.isEnhanced());

		// listeners
		Listener dirtyListener = new Listener() {
			@Override
			public void handleEvent(Event event) {
				if (!isDirty()) {
					markDirty();
				}
			}
		};
		for (StringConstraintComponent constraintComponent : constrainComponents) {
			constraintComponent.addDirtyListener(dirtyListener);
		}
		exceptionSimple.addListener(SWT.Selection, dirtyListener);
		exceptionEnhanced.addListener(SWT.Selection, dirtyListener);
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public void commit(boolean onSave) {
		if (onSave) {
			super.commit(onSave);

			for (StringConstraintComponent constraintComponent : constrainComponents) {
				constraintComponent.validateUpdate(true);
			}
			((ExceptionSensorConfig) environment.getExceptionSensorConfig()).setEnhanced(exceptionEnhanced.getSelection());
		}
	}

	/**
	 * @return Layout for radio group.
	 */
	private static Layout getLayoutForRadioGroup() {
		GridLayout gl = new GridLayout(2, true);
		gl.marginHeight = 0;
		gl.marginWidth = 0;
		return gl;
	}

	/**
	 * @return Return {@link GridData} with the horizontal ident.
	 */
	private static Object getIndentGridData() {
		GridData gd = new GridData(SWT.FILL, SWT.CENTER, true, false);
		gd.horizontalIndent = 20;
		return gd;
	}

	/**
	 * Creates info icon with given text as tool-tip.
	 *
	 * @param parent
	 *            Composite to create on.
	 * @param toolkit
	 *            {@link FormToolkit} to use.
	 * @param text
	 *            Information text.
	 */
	private static void createInfoLabel(Composite parent, FormToolkit toolkit, String text) {
		Label label = toolkit.createLabel(parent, "");
		label.setToolTipText(text);
		label.setImage(InspectIT.getDefault().getImage(InspectITImages.IMG_INFORMATION));
	}

	private static class DotNetStackTraceComponent extends StringConstraintComponent {

		private final Map<TriggerType, Button> triggerButtons = new HashMap<>();
		private final Map<StackTraceProviderType, Button> providerButtons = new HashMap<>();

		private Text samplingInterval;
		private ValidationControlDecoration<Text> samplingIntervalDecoration;

		private DotNetStackTraceSensorConfig sensorConfig;

		/**
		 * @param sensorConfig
		 * @param messageManager
		 */
		public DotNetStackTraceComponent(DotNetStackTraceSensorConfig sensorConfig, IMessageManager messageManager) {
			super(sensorConfig, messageManager);
			this.sensorConfig = sensorConfig;
		}

		/**
		 * {@inheritDoc}
		 */
		@Override
		public void createComponent(Composite parent, FormToolkit toolkit, String label, String infoText, int layoutColumns) {
			super.createComponent(parent, toolkit, label, infoText, layoutColumns);

			toolkit.createLabel(parent, "Trigger:").setLayoutData(getIndentGridData());
			Composite radioGroupHelp = toolkit.createComposite(parent);
			radioGroupHelp.setLayout(getLayoutForRadioGroup());
			radioGroupHelp.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));

			for (TriggerType triggerType : TriggerType.values()) {
				Button b = toolkit.createButton(radioGroupHelp, triggerType.getName(), SWT.RADIO);
				b.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));
				triggerButtons.put(triggerType, b);
			}

			samplingInterval = toolkit.createText(parent, "", SWT.BORDER | SWT.RIGHT);
			samplingInterval.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));
			createInfoLabel(parent, toolkit,
					"Defines the type of trigger for sampling. For time-based triggers, the sampling interval (the time between two samplings) in milliseconds has to be specified.");

			// decoration
			samplingIntervalDecoration = new ValidationControlDecoration<Text>(samplingInterval, super.messageManager) {
				@Override
				protected boolean validate(Text control) {
					return validateSamplingIntervalUpdate(false);
				}
			};
			samplingIntervalDecoration.setDescriptionText("Must be a positive number.");
			samplingIntervalDecoration.registerListener(SWT.Modify);

			fillTriggerSelectionAndSamplingInterval();

			// listeners
			Listener listener = new Listener() {
				@Override
				public void handleEvent(Event event) {
					boolean samplingIntervalRequired = false;

					for (Map.Entry<TriggerType, Button> entry : triggerButtons.entrySet()) {
						if (entry.getKey().dependsOnSamplingInterval() && entry.getValue().getSelection()) {
							samplingIntervalRequired = true;
							break;
						}
					}

					if (samplingIntervalRequired) {
						samplingInterval.setEnabled(true);
						samplingInterval.setText(String.valueOf(sensorConfig.getSamplingInterval()));
					} else {
						samplingInterval.setEnabled(false);
						samplingInterval.setText("");
					}
				}
			};

			for (Map.Entry<TriggerType, Button> entry : triggerButtons.entrySet()) {
				entry.getValue().addListener(SWT.Selection, listener);
			}

			// Provider
			toolkit.createLabel(parent, "Provider:").setLayoutData(getIndentGridData());
			radioGroupHelp = toolkit.createComposite(parent);
			radioGroupHelp.setLayout(getLayoutForRadioGroup());
			radioGroupHelp.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));

			boolean buttonSelected = false;
			for (StackTraceProviderType providerType : StackTraceProviderType.values()) {
				Button b = toolkit.createButton(radioGroupHelp, providerType.getName(), SWT.RADIO);
				b.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));
				providerButtons.put(providerType, b);

				if (providerType == sensorConfig.getProviderType()) {
					b.setSelection(true);
					buttonSelected = true;
				}
			}

			if (!buttonSelected) {
				sensorConfig.setProviderType(StackTraceProviderType.SHADOW_STACK);
				providerButtons.get(StackTraceProviderType.SHADOW_STACK).setSelection(true);
			}

			createInfoLabel(parent, toolkit, "Defines the stack trace provider. Besides retrieving the native stack of the CLR, alternatives like shadow stacks could be used.");
		}

		/**
		 * {@inheritDoc}
		 */
		@Override
		public boolean validateUpdate(boolean update) {
			boolean valid = super.validateUpdate(update);

			if (update) {
				valid &= validateSamplingIntervalUpdate(true);

				for (Map.Entry<StackTraceProviderType, Button> entry : providerButtons.entrySet()) {
					if (entry.getValue().getSelection()) {
						sensorConfig.setProviderType(entry.getKey());
						break;
					}
				}
			}

			return valid;
		}

		public boolean validateSamplingIntervalUpdate(boolean update) {
			boolean valid = true;

			for (Map.Entry<TriggerType, Button> entry : triggerButtons.entrySet()) {
				if (entry.getValue().getSelection()) {
					TriggerType triggerType = entry.getKey();
					if (triggerType.dependsOnSamplingInterval()) {
						try {
							int interval = Integer.parseInt(samplingInterval.getText());
							if (interval <= 0) {
								valid = false;
							} else {
								if (update) {
									sensorConfig.setSamplingInterval(interval);
									sensorConfig.setTriggerType(triggerType);
								}
							}
						} catch (NumberFormatException e) {
							valid = false;
						}
					} else {
						if (update) {
							sensorConfig.setSamplingInterval(0);
							sensorConfig.setTriggerType(triggerType);
						}
					}
					break;
				}
			}

			return valid;
		}

		private void fillTriggerSelectionAndSamplingInterval() {
			long interval = sensorConfig.getSamplingInterval();

			if (sensorConfig.getTriggerType() == null) {
				sensorConfig.setTriggerType(TriggerType.TIMER);
			}

			for (Map.Entry<TriggerType, Button> entry : triggerButtons.entrySet()) {
				TriggerType triggerType = entry.getKey();
				boolean selected = triggerType == sensorConfig.getTriggerType();
				entry.getValue().setSelection(selected);

				if (selected && triggerType.dependsOnSamplingInterval()) {
					samplingInterval.setText(String.valueOf(interval));
				} else if (selected && !triggerType.dependsOnSamplingInterval()) {
					samplingInterval.setText("");
					samplingInterval.setEnabled(false);
				}
			}
		}

		/**
		 * {@inheritDoc}
		 */
		@Override
		public void addDirtyListener(Listener listener) {
			super.addDirtyListener(listener);
			for (Map.Entry<TriggerType, Button> entry : triggerButtons.entrySet()) {
				entry.getValue().addListener(SWT.Selection, listener);
			}
			for (Map.Entry<StackTraceProviderType, Button> entry : providerButtons.entrySet()) {
				entry.getValue().addListener(SWT.Selection, listener);
			}
			samplingInterval.addListener(SWT.Modify, listener);
		}

		/**
		 * {@inheritDoc}
		 */
		@Override
		public boolean updateSensorConfig(StringConstraintSensorConfig sensorConfig) {
			boolean updated = super.updateSensorConfig(sensorConfig);

			if (updated && Objects.equals(this.sensorConfig.getClassName(), sensorConfig.getClassName())) {
				this.sensorConfig = (DotNetStackTraceSensorConfig) sensorConfig;
				return true;
			}

			return false;
		}

	}

	/**
	 * Help class for managing {@link StringConstraintSensorConfig}s.
	 *
	 * @author Ivan Senic
	 *
	 */
	private static class StringConstraintComponent {

		/**
		 * Sensor config.
		 */
		private StringConstraintSensorConfig sensorConfig;

		/**
		 * Message manager to use in the validation control decoration.
		 */
		private final IMessageManager messageManager;

		/**
		 * Unlimited button.
		 */
		private Button unlimited;

		/**
		 * Limited button.
		 */
		private Button limited;

		/**
		 * Value of string length.
		 */
		private Text value;

		/**
		 * {@link ControlDecoration} for displaying validation errors.
		 */
		private ValidationControlDecoration<Text> valueDecoration;

		/**
		 * Default constructor.
		 *
		 * @param sensorConfig
		 *            Sensor config.
		 * @param messageManager
		 *            Message manager to use in the validation control decoration.
		 */
		public StringConstraintComponent(StringConstraintSensorConfig sensorConfig, IMessageManager messageManager) {
			this.sensorConfig = sensorConfig;
			this.messageManager = messageManager;
		}

		/**
		 * Creates component.
		 *
		 * @param parent
		 *            Parent composite
		 * @param toolkit
		 *            {@link FormToolkit}m
		 * @param label
		 *            Main label text
		 * @param infoText
		 *            Info text
		 * @param layoutColumns
		 *            How much columns should component occupy.
		 */
		public void createComponent(Composite parent, FormToolkit toolkit, String label, String infoText, int layoutColumns) {
			// title part
			FormText timerText = toolkit.createFormText(parent, false);
			timerText.setText("<form><p><img href=\"img\" /> <b>" + TextFormatter.getSensorConfigName(sensorConfig) + "</b></p></form>", true, false);
			timerText.setImage("img", ImageFormatter.getSensorConfigImage(sensorConfig));
			timerText.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false, layoutColumns, 1));

			// widgets
			toolkit.createLabel(parent, label).setLayoutData(getIndentGridData());
			Composite radioGroupHelp = toolkit.createComposite(parent);
			radioGroupHelp.setLayout(getLayoutForRadioGroup());
			radioGroupHelp.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false, layoutColumns - 3, 1));
			unlimited = toolkit.createButton(radioGroupHelp, "Unlimited", SWT.RADIO);
			unlimited.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));
			limited = toolkit.createButton(radioGroupHelp, "Limited", SWT.RADIO);
			limited.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));
			value = toolkit.createText(parent, "", SWT.BORDER | SWT.RIGHT);
			value.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, false));
			createInfoLabel(parent, toolkit, infoText);

			// decoration
			valueDecoration = new ValidationControlDecoration<Text>(value, messageManager) {
				@Override
				protected boolean validate(Text control) {
					return validateUpdate(false);
				}
			};
			valueDecoration.setDescriptionText("Must be a positive number.");
			valueDecoration.registerListener(SWT.Modify);

			// fill with current value
			fillValue();

			// listeners
			Listener listener = new Listener() {
				@Override
				public void handleEvent(Event event) {
					if (limited.getSelection()) {
						value.setEnabled(true);
						value.setText(String.valueOf(sensorConfig.getStringLength()));
					} else {
						value.setEnabled(false);
						value.setText("");
					}
				}
			};
			unlimited.addListener(SWT.Selection, listener);
			limited.addListener(SWT.Selection, listener);
		}

		/**
		 * Adds dirty listener to needed controls.
		 *
		 * @param listener
		 *            Dirty listener.
		 */
		public void addDirtyListener(Listener listener) {
			unlimited.addListener(SWT.Selection, listener);
			limited.addListener(SWT.Selection, listener);
			value.addListener(SWT.Modify, listener);
		}

		/**
		 * Validates the value and updates if needed.
		 *
		 * @param update
		 *            If beside validation an update on the model object should be done.
		 * @return if value in the control is valid
		 */
		public boolean validateUpdate(boolean update) {
			boolean valid = true;
			if (unlimited.getSelection()) {
				if (update) {
					sensorConfig.setStringLength(0);
				}
			} else {
				try {
					int length = Integer.parseInt(value.getText());
					if (length <= 0) {
						valid = false;
					} else {
						if (update) {
							sensorConfig.setStringLength(length);
						}
					}
				} catch (NumberFormatException e) {
					valid = false;
				}
			}
			return valid;
		}

		/**
		 * Fills the config value.
		 */
		private void fillValue() {
			int length = sensorConfig.getStringLength();
			if (length > 0) {
				unlimited.setSelection(false);
				limited.setSelection(true);
				value.setText(String.valueOf(length));
			} else {
				unlimited.setSelection(true);
				limited.setSelection(false);
				value.setText("");
				value.setEnabled(false);
			}
		}

		/**
		 * Updates the sensor config if it's relating to the same class.
		 *
		 * @param sensorConfig
		 *            new config
		 * @return <code>true</code> if update occurred
		 */
		public boolean updateSensorConfig(StringConstraintSensorConfig sensorConfig) {
			if (Objects.equals(this.sensorConfig.getClassName(), sensorConfig.getClassName())) {
				this.sensorConfig = sensorConfig;
				return true;
			}

			return false;
		}

	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public void propertyChanged(Object source, int propId) {
		if (propId == IEditorPart.PROP_INPUT) {
			EnvironmentEditorInput input = (EnvironmentEditorInput) formPage.getEditor().getEditorInput();
			environment = input.getEnvironment();

			for (StringConstraintComponent constraintComponent : constrainComponents) {
				for (IMethodSensorConfig methodSensorConfig : environment.getMethodSensorConfigs()) {
					if (methodSensorConfig instanceof StringConstraintSensorConfig) {
						if (constraintComponent.updateSensorConfig((StringConstraintSensorConfig) methodSensorConfig)) {
							break;
						}
					}
				}
				// manually exception sensor
				constraintComponent.updateSensorConfig((StringConstraintSensorConfig) environment.getExceptionSensorConfig());
			}
		}
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public void dispose() {
		formPage.getEditor().removePropertyListener(this);
		super.dispose();
	}

}
