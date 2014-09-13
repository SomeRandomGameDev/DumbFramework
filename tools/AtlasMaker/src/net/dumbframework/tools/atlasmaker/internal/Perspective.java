package net.dumbframework.tools.atlasmaker.internal;

import org.eclipse.swt.SWT;
import org.eclipse.ui.IPageLayout;
import org.eclipse.ui.IPerspectiveFactory;

public class Perspective implements IPerspectiveFactory {

	public void createInitialLayout(IPageLayout layout) {
		layout.setEditorAreaVisible(false);
		layout.addStandaloneView("ATLASMAKER_IMAGE_VIEWER", true, SWT.LEFT, 0.95f, null);
	}

}
