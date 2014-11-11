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

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.graphics.Rectangle;

/**
 * Atlas.
 * @author Stoned Xander
 */
public class Atlas {

    /**
     * Default Atlas size (in pixels).
     */
    public static final int DEFAULT_SIZE = 2048;

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

        /**
         * Fill the layer composition with nodes in root.
         * @param root Node root.
         */
        protected void fillWithNodes(Node root) {
            composition.clear();
            root.fillResult(composition);
        }
    }

    /**
     * Node. Temporary structure for Atlas Building.
     * @author Stoned Xander.
     */
    private static class Node {
        /**
         * Sub-nodes.
         */
        private Node[] subs = new Node[2];
        /**
         * Concerned sprite.
         */
        private Sprite sprite;
        /**
         * Actual placeholder.
         */
        private Rectangle placeholder;

        /**
         * Constructor.
         * @param pholder Place holder.
         */
        public Node(Rectangle pholder) {
            placeholder = pholder;
        }

        /**
         * Insert the sprite in the sub-tree.
         * @param spr Sprite to insert.
         * @return The actual node used for sprite insertion.
         */
        public Node insert(Sprite spr) {
            Node result = null;
            if ((null != subs[0]) && (null != subs[1])) {
                // We're not in a leaf.
                Node newNode = subs[0].insert(spr);
                if (null == newNode) {
                    newNode = subs[1].insert(spr);
                }
                result = newNode;
            } else {
                if (null == sprite) {
                    // Let's check the actual space.
                    Rectangle rect = spr.getBounds();
                    int rw = rect.width - rect.x;
                    int rh = rect.height - rect.y;
                    if ((placeholder.width < rw) || (placeholder.height < rh)) {
                        // Too small.
                        result = null;
                    } else if ((rw == placeholder.width) && (rh == placeholder.height)) {
                        sprite = spr;
                        result = this;
                    } else {
                        // There's too much room. We must split.
                        int dw = placeholder.width - rw;
                        int dh = placeholder.height - rh;

                        if (dw > dh) { // Some KD-tree like decision.
                            subs[0] = new Node(new Rectangle(placeholder.x, placeholder.y, rw,
                                    placeholder.height));
                            subs[1] = new Node(new Rectangle(placeholder.x + rw + 1, placeholder.y,
                                    placeholder.width - rw - 1, placeholder.height));
                        } else {
                            subs[0] = new Node(new Rectangle(placeholder.x, placeholder.y, placeholder.width,
                                    rh));
                            subs[1] = new Node(new Rectangle(placeholder.x, placeholder.y + rh + 1,
                                    placeholder.width, placeholder.height - rh - 1));
                        }
                        result = subs[0].insert(spr);
                    }
                } else {
                    // There's no room here.
                    // We're in a leaf.
                    result = null;
                }
            }
            return result;
        }

        /**
         * Fill the result map with the stored sprites in the sub-tree.
         * @param result Result map to fill.
         */
        public void fillResult(Map<Sprite, Point> result) {
            if (sprite != null) {
                result.put(sprite, new Point(placeholder.x, placeholder.y));
            } else {
                if (subs[0] != null) {
                    subs[0].fillResult(result);
                }
                if (subs[1] != null) {
                    subs[1].fillResult(result);
                }
            }
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
        clear();
        // First, get sprites and sort them by "size".
        Map<String, Sprite> sprites = SpriteManager.getDefault().getSprites();
        List<Sprite> sortedSprites = new ArrayList<Sprite>(sprites.values());
        Collections.sort(sortedSprites, new Comparator<Sprite>() {
            @Override
            public int compare(Sprite o1, Sprite o2) {
                Rectangle rect = o1.getBounds();
                long size1 = rect.x * rect.y;
                rect = o2.getBounds();
                long size2 = rect.x * rect.y;
                int result = Long.compare(size1, size2);
                return result;
            }
        });
        List<Node> roots = new LinkedList<Node>();
        Node currentRoot = null;
        for (Sprite i : sortedSprites) {
            Node success = null;
            if (null != currentRoot) {
                success = currentRoot.insert(i);
            }
            if (null == success) {
                currentRoot = new Node(new Rectangle(0, 0, DEFAULT_SIZE, DEFAULT_SIZE));
                roots.add(currentRoot);
                success = currentRoot.insert(i);
                if(null == success) {
                    // We can't do more.
                    throw new IllegalStateException();
                }
            }
        }

        // For every root, create a layer.
        for (Node i : roots) {
            Layer layer = new Layer();
            layer.fillWithNodes(i);
            layers.add(layer);
        }
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
