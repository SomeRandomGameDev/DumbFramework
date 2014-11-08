/**
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * * Neither the name of the University of California, Berkeley nor the
 *   names of its contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
package org.dumbframework.tools.atlasmaker.model;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import org.eclipse.swt.graphics.Image;

/**
 * Sprite Manager.
 * @author Stoned Xander
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

    /**
     * Register a sprite.
     * @param sprite Sprite to register.
     */
    public void register(Sprite sprite) {
        String identifier = sprite.getIdentifier();
        sprites.put(identifier, sprite);
        images.put(identifier, sprite.getImage());
    }

    /**
     * Modify the sprite reference
     * @param sprite Sprite to modify.
     * @param newIdentifier New sprite identifier.
     */
    public void modifyReference(Sprite sprite, String newIdentifier) {
        String oldIdentifier = sprite.getIdentifier();
        sprites.remove(oldIdentifier);
        images.remove(oldIdentifier);
        sprite.setIdentifier(newIdentifier); // Not very elegant.
        register(sprite);
    }

    /**
     * Delete a sprite.
     * @param sprite Sprite to delete.
     */
    public void deleteSprite(Sprite sprite) {
        String identifier = sprite.getIdentifier();
        sprites.remove(identifier);
        Image image = images.get(identifier);
        images.remove(identifier);
        if (!images.containsValue(image)) {
            image.dispose();
        }
    }

    /**
     * @return A list of identifier.
     */
    public Set<String> getIdentifiers() {
        Set<String> result = Collections.unmodifiableSet(sprites.keySet());
        return result;
    }

    /**
     * @return The sprite collection.
     */
    public Map<String, Sprite> getSprites() {
        Map<String, Sprite> result = Collections.unmodifiableMap(sprites);
        return result;
    }
}
