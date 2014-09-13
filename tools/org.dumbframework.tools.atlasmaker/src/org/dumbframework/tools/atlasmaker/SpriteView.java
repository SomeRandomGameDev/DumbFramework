package org.dumbframework.tools.atlasmaker;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.util.Map;
import java.util.Set;

import org.dumbframework.tools.atlasmaker.model.Sprite;
import org.dumbframework.tools.atlasmaker.model.SpriteManager;
import org.eclipse.jface.dialogs.IInputValidator;
import org.eclipse.jface.dialogs.InputDialog;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.PaintEvent;
import org.eclipse.swt.events.PaintListener;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.ImageData;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.widgets.Canvas;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.widgets.MenuItem;
import org.eclipse.swt.widgets.Sash;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableItem;
import org.eclipse.swt.widgets.Widget;
import org.eclipse.ui.part.ViewPart;

public class SpriteView extends ViewPart {

	/**
	 * Identifier.
	 */
	public static final String ID = "org.dumbframework.tools.atlasmaker.SpriteView";

	/**
	 * Exploitable image extensions.
	 */
	private static final String[] IMAGE_EXTENSIONS = { "*.png", "*.jpg",
	        "*.gif" };

	/**
	 * Color RED.
	 */
	private static final Color RED = Display.getCurrent().getSystemColor(SWT.COLOR_RED);
	
	/**
	 * Input validator for sprite identifier change.
	 */
	private IInputValidator identifierValidator = new IInputValidator() {

		@Override
		public String isValid(String newText) {
			String result = null;
			Set<String> existing = SpriteManager.getDefault().getIdentifiers();
			if (newText.isEmpty() || existing.contains(newText)) {
				result = "Invalid identifier";
			}
			return result;
		}

	};

	/**
	 * Image to display in the canvas.
	 */
	private Image toDisplay;
	
	/**
	 * Effective current sprite area.
	 */
	private Rectangle area;

	public SpriteView() {
		// Some initialization about images.
	}

	@Override
	public void createPartControl(Composite parent) {
		Composite main = new Composite(parent, SWT.NONE);
		main.setLayout(new FormLayout());
		final Table imageList = new Table(main, SWT.BORDER);
		final Sash sash = new Sash(main, SWT.VERTICAL);
		final Composite displayPanel = new Composite(main, SWT.BORDER);
		final Canvas canvas = new Canvas(displayPanel, SWT.BORDER);
		final Sash sashPanel = new Sash(displayPanel, SWT.HORIZONTAL);
		final Composite infoPanel = new Composite(displayPanel, SWT.BORDER);

		// Layout data for Table.
		FormData data = new FormData();
		data.left = new FormAttachment(0, 0);
		data.right = new FormAttachment(sash, 0);
		data.top = new FormAttachment(0, 0);
		data.bottom = new FormAttachment(100, 0);
		imageList.setLayoutData(data);

		// Layout data for sash between table and display panel.
		data = new FormData();
		data.left = new FormAttachment(25, 0);
		data.top = new FormAttachment(0, 0);
		data.bottom = new FormAttachment(100, 0);
		sash.setLayoutData(data);
		sash.addListener(SWT.Selection, new Listener() {
			@Override
			public void handleEvent(Event event) {
				Widget widget = event.widget;
				if (widget instanceof Sash) {
					Sash sashItem = (Sash) widget;
					FormData sashData = (FormData) sashItem.getLayoutData();
					Rectangle sashRect = sashItem.getBounds();
					Composite parent = sashItem.getParent();
					Rectangle parentRect = parent.getClientArea();
					int right = parentRect.width - sashRect.width - 256;
					event.x = Math.max(Math.min(event.x, right), 256);
					if (event.x != sashRect.x) {
						sashData.left = new FormAttachment(0, event.x);
						parent.layout();
					}
				}
			}
		});

		// Layout data for display panel.
		data = new FormData();
		data.left = new FormAttachment(sash, 0);
		data.top = new FormAttachment(0, 0);
		data.bottom = new FormAttachment(100, 0);
		data.right = new FormAttachment(100, 0);
		displayPanel.setLayoutData(data);

		displayPanel.setLayout(new FormLayout());

		// Layout data for canvas.
		data = new FormData();
		data.left = new FormAttachment(0, 0);
		data.right = new FormAttachment(100, 0);
		data.top = new FormAttachment(0, 0);
		data.bottom = new FormAttachment(sashPanel, 0);
		canvas.setLayoutData(data);

		// Layout data for sash between canvas and info panel.
		data = new FormData();
		data.left = new FormAttachment(0, 0);
		data.top = new FormAttachment(80, 0);
		data.right = new FormAttachment(100, 0);
		sashPanel.setLayoutData(data);
		sashPanel.addListener(SWT.Selection, new Listener() {
			@Override
			public void handleEvent(Event event) {
				Widget widget = event.widget;
				if (widget instanceof Sash) {
					Sash sashItem = (Sash) widget;
					FormData sashData = (FormData) sashItem.getLayoutData();
					Rectangle sashRect = sashItem.getBounds();
					Composite parent = sashItem.getParent();
					Rectangle parentRect = parent.getClientArea();
					int bottom = parentRect.height - sashRect.height - 128;
					event.y = Math.max(Math.min(event.y, bottom), 128);
					if (event.y != sashRect.y) {
						sashData.top = new FormAttachment(0, event.y);
						parent.layout();
					}
				}
			}
		});

		// Layout data for info panel.
		data = new FormData();
		data.left = new FormAttachment(0, 0);
		data.top = new FormAttachment(sashPanel, 0);
		data.bottom = new FormAttachment(100, 0);
		data.right = new FormAttachment(100, 0);
		infoPanel.setLayoutData(data);

		parent.setLayout(new FillLayout());
		main.pack();

		// Add menus.
		final Menu tableMenu = new Menu(imageList);
		imageList.setMenu(tableMenu);
		final MenuItem addSprite = new MenuItem(tableMenu, SWT.PUSH);
		addSprite.setText("Add Sprite");
		addSprite.setImage(Activator.getDefault().getBundleImage(
		        "icons/image_add.png"));
		addSprite.addListener(SWT.Selection, new Listener() {
			@Override
			public void handleEvent(Event event) {
				SpriteView.this.addSprite(imageList);
			}
		});
		final MenuItem addPack = new MenuItem(tableMenu, SWT.PUSH);
		addPack.setText("Add Image Pack");
		addPack.setImage(Activator.getDefault().getBundleImage(
		        "icons/images.png"));
		addPack.addListener(SWT.Selection, new Listener() {
			@Override
			public void handleEvent(Event event) {
				SpriteView.this.addPack(imageList);
			}
		});
		final MenuItem removeSprite = new MenuItem(tableMenu, SWT.PUSH);
		removeSprite.setText("Remove Sprite");
		removeSprite.setImage(Activator.getDefault().getBundleImage(
		        "icons/image_delete.png"));
		removeSprite.addListener(SWT.Selection, new Listener() {
			@Override
			public void handleEvent(Event event) {
				if (imageList.getSelectionCount() > 0) {
					TableItem[] selection = imageList.getSelection();
					for (TableItem i : selection) {
						Sprite sprite = (Sprite) i.getData();
						SpriteView.this.removeSprite(sprite);
					}
					int[] indices = imageList.getSelectionIndices();
					imageList.remove(indices);
					canvas.redraw();
				}
			}
		});
		new MenuItem(tableMenu, SWT.SEPARATOR);
		final MenuItem editSprite = new MenuItem(tableMenu, SWT.PUSH);
		editSprite.setText("Change Identifier");
		editSprite.setImage(Activator.getDefault().getBundleImage(
		        "icons/image_edit.png"));
		editSprite.addListener(SWT.Selection, new Listener() {
			@Override
			public void handleEvent(Event event) {
				if (imageList.getSelectionCount() > 0) {
					SpriteView.this.editSprite(imageList.getSelection()[0]);
				}
			}
		});

		imageList.addListener(SWT.Selection, new Listener() {
			@Override
			public void handleEvent(Event event) {
				if (imageList.getSelectionCount() > 0) {
					TableItem item = imageList.getSelection()[0];
					Sprite sprite = (Sprite) item.getData();
					toDisplay = sprite.getImage();
					area = sprite.getBounds();
					canvas.redraw();
				}
			}
		});

		canvas.addPaintListener(new PaintListener() {

			@Override
			public void paintControl(PaintEvent e) {
				if (!(toDisplay == null || toDisplay.isDisposed())) {
					e.gc.setForeground(RED);
					e.gc.drawRectangle(area);
					e.gc.drawImage(toDisplay, 0, 0);
				}
			}
		});

		Map<String, Sprite> sprites = SpriteManager.getDefault().getSprites();
		for(Map.Entry<String, Sprite> i : sprites.entrySet()) {
			TableItem tableItem = new TableItem(imageList, SWT.NONE);
			tableItem.setText(i.getKey());
			tableItem.setData(i.getValue());
		}
		
	}

	/**
	 * Take care of the loading and storing of a sprite.
	 * 
	 * @param table
	 *            Table in which to add the sprite.
	 */
	protected void addSprite(Table table) {
		String path = chooseFile();

		if (path != null) {
			Set<String> identifiers = SpriteManager.getDefault()
			        .getIdentifiers();
			if (!identifiers.contains(path)) {
				try {
					InputStream stream = new FileInputStream(new File(path));
					if (stream != null) {
						Image image = new Image(this.getViewSite().getShell()
						        .getDisplay(), stream);
						Rectangle bounds = image.getBounds();
						ImageData data = image.getImageData();
						int minX = bounds.width;
						int minY = bounds.height;
						int maxX = 0;
						int maxY = 0;
						for(int i = 0; i < bounds.width; ++i) {
							for(int j = 0; j < bounds.height; ++j) {
								int alpha = data.getAlpha(i, j);
								if(alpha != 0) {
									// We got a pixel at least here !
									if(i < minX) {
										minX = i;
									} else if(i > maxX) {
										maxX = i;
									}
									if(j < minY) {
										minY = j;
									} else if(j > maxY) {
										maxY = j;
									}
								}
							}
						}
						// SO ! Trimming !
						Rectangle trimmed = new Rectangle(minX, minY, maxX - minX, maxY - minY);
						Sprite sprite = new Sprite(image, path, trimmed);
						TableItem tableItem = new TableItem(table, SWT.NONE);
						tableItem.setText(path);
						tableItem.setData(sprite);
					}
					stream.close();
				} catch (Exception ex) {
					// TODO Log Exception !
					ex.printStackTrace();
				}
			}
		}
	}

	/**
	 * Open a dialog to choose an image file.
	 * 
	 * @return Path to an image file if any.
	 */
	private String chooseFile() {
		FileDialog dialog = new FileDialog(this.getViewSite().getShell(),
		        SWT.OPEN);
		dialog.setFilterExtensions(IMAGE_EXTENSIONS);

		String path = dialog.open();
		return path;
	}

	/**
	 * Take care of the loading and storing of a sprite pack.
	 * 
	 * @param table
	 *            Table in which to add the sprites.
	 */
	protected void addPack(Table table) {
		// Seriously ? This will be EPIC.
		
		// TODO EPIC stuff to do here. Sprite isolation, auto trimming.
	}

	/**
	 * Take care of the removal of a sprite.
	 * 
	 * @param sprite
	 *            Sprite to remove from the sprite manager.
	 */
	protected void removeSprite(Sprite sprite) {
		sprite.dispose();
	}

	/**
	 * Edit the sprite name.
	 * 
	 * @param sprite
	 *            Targeted sprite.
	 */
	protected void editSprite(TableItem item) {
		Sprite sprite = (Sprite) item.getData();

		InputDialog dialog = new InputDialog(getViewSite().getShell(),
		        "Change Sprite identifier", "Enter a value",
		        sprite.getIdentifier(), identifierValidator);

		if (dialog.open() == 0) {
			String value = dialog.getValue();
			if (value != null) {
				SpriteManager.getDefault().modifyReference(sprite, value);
				item.setText(value);
			}
		}

	}

	@Override
	public void setFocus() {
		// Nothing to do.
	}

}
