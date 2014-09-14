/**
 * 
 */
package org.dumbframework.tools.atlasmaker.model;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import org.eclipse.swt.graphics.Image;

/**
 * Sprite Manager.
 * 
 * @author xander
 */
public class SpriteManager {

	/**
	 * Unique instance of the manager.
	 */
	private static final SpriteManager INSTANCE = new SpriteManager();

	/**
	 * Link from Sprite identifier to their definition.
	 */
	private Map<String, Sprite> sprites = new HashMap<String, Sprite>();

	/**
	 * References to images for life-cycle management.
	 */
	private Map<String, Image> images = new HashMap<String, Image>();

	/**
	 * Lock constructor.
	 */
	private SpriteManager() {
		// Nothing to do.
	}

	/**
	 * @return THE Sprite manager.
	 */
	public static SpriteManager getDefault() {
		return INSTANCE;
	}

	public void register(Sprite sprite) {
		String identifier = sprite.getIdentifier();
		sprites.put(identifier, sprite);
		images.put(identifier, sprite.getImage());
	}

	public void modifyReference(Sprite sprite, String newIdentifier) {
		String oldIdentifier = sprite.getIdentifier();
		sprites.remove(oldIdentifier);
		images.remove(oldIdentifier);
		sprite.setIdentifier(newIdentifier); // Not very elegant.
		register(sprite);
	}

	public void deleteSprite(Sprite sprite) {
		String identifier = sprite.getIdentifier();
		sprites.remove(identifier);
		Image image = images.get(identifier);
		images.remove(identifier);
		if (!images.containsValue(image)) {
			image.dispose();
		}
	}

	public Set<String> getIdentifiers() {
		Set<String> result = Collections.unmodifiableSet(sprites.keySet());
		return result;
	}
	
	public Map<String, Sprite> getSprites() {
		Map<String, Sprite> result = Collections.unmodifiableMap(sprites);
		return result;
	}
}
