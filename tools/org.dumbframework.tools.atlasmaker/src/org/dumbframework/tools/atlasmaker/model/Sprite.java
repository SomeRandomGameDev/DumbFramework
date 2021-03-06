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

import java.util.HashMap;
import java.util.Map;

import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.graphics.Rectangle;

/**
 * Model for sprite. Manages identifier, trim, memory storage.
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
     * Anchor points.
     */
    private Map<String, Point> anchors;

    /**
     * Constructor.
     * @param img Support image.
     * @param id Identifier.
     * @param bds Bounds.
     */
    public Sprite(Image img, String id, Rectangle bds) {
        image = img;
        identifier = id;
        bounds = bds;
        anchors = new HashMap<String, Point>();
        SpriteManager.getDefault().register(this);
    }

    /**
     * @return The Identifier.
     */
    public String getIdentifier() {
        return identifier;
    }

    /**
     * Set the sprite identifier.
     * @param identifier The sprite identifier.
     */
    public void setIdentifier(String identifier) {
        this.identifier = identifier;
    }

    /**
     * Set the anchor coordinate.
     * @param id Anchor identifier.
     * @param pt Associated container..
     */
    public void setAnchor(String id, Point pt) {
        if (null == pt) {
            anchors.remove(id);
        }
    }

    /**
     * @param id Anchor identifier.
     * @return The container of the identified anchor.
     */
    public Point getAnchor(String id) {
        Point result = anchors.get(id);
        return result;
    }

    /**
     * Fetch the anchors.
     * @param buffer Container in which to store anchors.
     */
    public void fetchAnchors(Map<String, Point> buffer) {
        if (null != buffer) {
            buffer.clear();
            buffer.putAll(anchors);
        }
    }

    /**
     * @return The image.
     */
    public Image getImage() {
        return image;
    }

    /**
     * @return The bounds.
     */
    public Rectangle getBounds() {
        return bounds;
    }

    /**
     * Dispose.
     */
    public void dispose() {
        SpriteManager.getDefault().deleteSprite(this);
        image = null;
        identifier = null;
        bounds = null;
        anchors.clear();
        anchors = null;
    }
}
