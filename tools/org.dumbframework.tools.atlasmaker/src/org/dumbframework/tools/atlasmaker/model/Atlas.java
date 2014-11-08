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
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.eclipse.swt.graphics.Point;

/**
 * Atlas.
 * @author Stoned Xander
 */
public class Atlas {

    /**
     * Default Atlas size (in pixels).
     */
    private static final int DEFAULT_SIZE = 2048;

    /**
     * Atlas Layer.
     * @author Stoned Xander
     */
    public class Layer {
        /**
         * Composition of the layer. It corresponds to a set of sprite attached to a position. The position
         * symbolized the upper-left corner of the sprite.
         */
        private Map<Sprite, Point> composition = new HashMap<Sprite, Point>();

        /**
         * Constructor.
         */
        public Layer() {
            // Nothing to do yet.
        }

        /**
         * Fetch the composition of the layer.
         * @param buffer Container in which to store the layer composition.
         */
        public void fetchComposition(Map<Sprite, Point> buffer) {
            if (null != buffer) {
                buffer.clear();
                buffer.putAll(composition);
            }
        }

        /**
         * Dispose references.
         */
        public void dispose() {
            composition.clear();
            composition = null;
        }
    }

    /**
     * Instance.
     */
    private static Atlas instance = new Atlas();

    /**
     * Size of the size of the atlas in pixels. An atlas is always a square which side is a power of two.
     */
    private int size = DEFAULT_SIZE;

    /**
     * List of layers.
     */
    private List<Layer> layers = new LinkedList<Layer>();

    /**
     * Constructor.
     */
    private Atlas() {
        // Nothing to do yet.
    }

    /**
     * @return The Atlas Instance.
     */
    public static Atlas getDefault() {
        return instance;
    }

    /**
     * Rebuild the atlas from the stored sprites.
     */
    public void rebuild() {
        // TODO
    }

    /**
     * Fetch the atlas layers.
     * @param buffer Container in which to store the layers.
     */
    public void fetchLayers(List<Layer> buffer) {
        synchronized (layers) {
            if (null != buffer) {
                buffer.clear();
                buffer.addAll(layers);
            }
        }
    }

    /**
     * @return The size of the side of the atlas (in pixels).
     */
    public int size() {
        return size;
    }

    /**
     * Clear the atlas.
     */
    public void clear() {
        synchronized (layers) {
            for (Layer i : layers) {
                i.dispose();
            }
            layers.clear();
        }
    }
}
