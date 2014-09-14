/**
 * 
 */
package org.dumbframework.tools.atlasmaker.model;

import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.Rectangle;

/**
 * Model for sprite. Manages identifier, trim, memory storage.
 * 
 * @author xander
 */
public class Sprite {
	/**
	 * Stored image.
	 */
	private Image image;
	/**
	 * Identifier.
	 */
	private String identifier;
	/**
	 * Trim.
	 */
	private Rectangle bounds;

	/**
	 * Constructor.
	 * 
	 * @param img
	 *            Support image.
	 * @param id
	 *            Identifier.
	 * @param bds
	 *            Bounds.
	 */
	public Sprite(Image img, String id, Rectangle bds) {
		image = img;
		identifier = id;
		bounds = bds;
		SpriteManager.getDefault().register(this);
	}

	public String getIdentifier() {
		return identifier;
	}

	public void setIdentifier(String identifier) {
		this.identifier = identifier;
	}

	public Image getImage() {
		return image;
	}

	public Rectangle getBounds() {
		return bounds;
	}

	public void dispose() {
		SpriteManager.getDefault().deleteSprite(this);
		image = null;
		identifier = null;
		bounds = null;
	}
}
