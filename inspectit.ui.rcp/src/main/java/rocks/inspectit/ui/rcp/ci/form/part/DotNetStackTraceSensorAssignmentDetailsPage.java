/**
 *
 */
package rocks.inspectit.ui.rcp.ci.form.part;

import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.viewers.StructuredSelection;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.ui.forms.IManagedForm;
import org.eclipse.ui.forms.widgets.FormToolkit;
import org.eclipse.ui.forms.widgets.Section;
import org.eclipse.ui.forms.widgets.TableWrapData;
import org.eclipse.ui.forms.widgets.TableWrapLayout;

import rocks.inspectit.shared.cs.ci.assignment.impl.DotNetStackTraceSensorAssignment;
import rocks.inspectit.shared.cs.ci.assignment.impl.MethodSensorAssignment;

/**
 * @author Henning Schulz
 *
 */
public class DotNetStackTraceSensorAssignmentDetailsPage extends MethodSensorAssignmentDetailsPage {

	private final MethodSensorAssignmentDetailsPage baseMethodAssignmentDetailsPage;

	private DotNetStackTraceSensorAssignment assignment;

	/**
	 * Constructor.
	 *
	 * @param masterBlockListener
	 *            listener to inform the master block on changes to the input
	 * @param canEdit
	 *            If the data can be edited.
	 */
	public DotNetStackTraceSensorAssignmentDetailsPage(ISensorAssignmentUpdateListener masterBlockListener, boolean canEdit) {
		super(masterBlockListener, canEdit);
		baseMethodAssignmentDetailsPage = new MethodSensorAssignmentDetailsPage(masterBlockListener, canEdit);
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public void initialize(IManagedForm form) {
		super.initialize(form);
		baseMethodAssignmentDetailsPage.initialize(form);
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	public void createContents(Composite parent) {
		TableWrapLayout parentLayout = new TableWrapLayout();
		parentLayout.topMargin = 5;
		parentLayout.leftMargin = 5;
		parentLayout.rightMargin = 2;
		parentLayout.bottomMargin = 2;
		parentLayout.numColumns = 2;
		parentLayout.makeColumnsEqualWidth = true;
		parent.setLayout(parentLayout);

		FormToolkit toolkit = managedForm.getToolkit();

		// abstract method definition
		super.createContents(parent, false);

		// special sensor definitions
		// section
		Section section = toolkit.createSection(parent, Section.TITLE_BAR | Section.EXPANDED);
		section.setText("Base method definition");
		section.marginWidth = 10;
		section.marginHeight = 5;
		TableWrapData td = new TableWrapData(TableWrapData.FILL, TableWrapData.TOP);
		td.grabHorizontal = true;
		section.setLayoutData(td);

		// main composite
		Composite mainComposite = toolkit.createComposite(section);
		GridLayout layout = new GridLayout(7, false);
		layout.marginHeight = 5;
		layout.marginWidth = 5;
		mainComposite.setLayout(layout);
		section.setClient(mainComposite);

		// title (TODO: do not show this title)
		baseMethodAssignmentDetailsPage.createTitle(parent, toolkit);

		baseMethodAssignmentDetailsPage.addClassContents(mainComposite);
		baseMethodAssignmentDetailsPage.addMethodContents(mainComposite);
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	protected void updateFromInput() {
		super.updateFromInput();
		baseMethodAssignmentDetailsPage.update();
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	protected void commitToInput() {
		super.commitToInput();
		baseMethodAssignmentDetailsPage.commitToInput();
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	protected MethodSensorAssignment getInput() {
		return assignment;
	}

	/**
	 * {@inheritDoc}
	 */
	@Override
	protected void setInput(ISelection selection) {
		if (!selection.isEmpty()) {
			assignment = (DotNetStackTraceSensorAssignment) ((IStructuredSelection) selection).getFirstElement();
			baseMethodAssignmentDetailsPage.setInput(new StructuredSelection(assignment.getBaseMethodAssignment()));
		} else {
			assignment = null; // NOPMD
		}
	}

}
