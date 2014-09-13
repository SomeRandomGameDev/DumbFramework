package org.dumbframework.tools.atlasmaker;

import org.eclipse.ui.IPageLayout;
import org.eclipse.ui.IPerspectiveFactory;

public class MainPerspective implements IPerspectiveFactory {

	public void createInitialLayout(IPageLayout layout) {
        String editorArea = layout.getEditorArea();
        layout.setEditorAreaVisible(false);

        // TODO Add an application argument for debugging.

        layout.addView(SpriteView.ID, IPageLayout.LEFT, 1.0f, editorArea);
	}

}
