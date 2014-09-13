package net.dumbframework.tools.atlasmaker.internal;

import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.widgets.Canvas;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Sash;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.Widget;
import org.eclipse.ui.part.ViewPart;

public class ImageViewer extends ViewPart {

	public ImageViewer() {
		// Some initialization about images.
	}

	@Override
	public void createPartControl(Composite parent) {
		Composite main = new Composite(parent, SWT.NONE);
		main.setLayout(new FormLayout());
		final Table imageList = new Table(main, SWT.BORDER);
		final Sash sash = new Sash(main, SWT.VERTICAL);
		final Canvas canvas = new Canvas(main, SWT.BORDER);
		
		// Layout data for Table.
		FormData data = new FormData();
		data.left = new FormAttachment(0, 0);
		data.right = new FormAttachment(sash, 0);
		data.top = new FormAttachment(0, 0);
		data.bottom = new FormAttachment(100, 0);
		imageList.setLayoutData(data);;
		
		data = new FormData();
		data.left = new FormAttachment(20, 0);
		data.top = new FormAttachment(0, 0);
		data.bottom = new FormAttachment(100, 0);
		sash.setLayoutData(data);
		sash.addListener(SWT.Selection, new Listener() {
			@Override
			public void handleEvent(Event event) {
				Widget widget = event.widget;
				if(widget instanceof Sash) {
					Sash sashItem = (Sash) widget;
					FormData sashData = (FormData) sashItem.getLayoutData();
					Rectangle sashRect = sashItem.getBounds ();
					Composite parent = sashItem.getParent();
					Rectangle parentRect = parent.getClientArea ();
					int right = parentRect.width - sashRect.width - 20;
					event.x = Math.max (Math.min (event.x, right), 20);
					if (event.x != sashRect.x)  {
						sashData.left = new FormAttachment (0, event.x);
						parent.layout ();
					}
				}
			}
		});
		
		data = new FormData();
		data.left = new FormAttachment(sash, 0);
		data.top = new FormAttachment(0, 0);
		data.bottom = new FormAttachment(100, 0);
		data.right = new FormAttachment(100, 0);
		canvas.setLayoutData(data);
		
		parent.setLayout(new FillLayout());
		main.pack();
	}

	@Override
	public void setFocus() {
		// TODO Stub de la méthode généré automatiquement

	}

}