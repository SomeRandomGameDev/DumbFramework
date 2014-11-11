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
package org.dumbframework.tools.atlasmaker;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.dumbframework.tools.atlasmaker.model.Atlas;
import org.dumbframework.tools.atlasmaker.model.Sprite;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.PaintEvent;
import org.eclipse.swt.events.PaintListener;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.GC;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.graphics.Rectangle;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Canvas;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.ScrollBar;
import org.eclipse.swt.widgets.TabFolder;
import org.eclipse.swt.widgets.TabItem;
import org.eclipse.ui.part.ViewPart;

/**
 * Atlas Builder.
 * @author Stoned Xander
 */
public class AtlasBuilder extends ViewPart {

    /**
     * Identifier.
     */
    public static final String ID = "org.dumbframework.tools.atlasmaker.AtlasBuilder"; //$NON-NLS-1$

    /**
     * Background color.
     */
    public static final Color BACKGROUND = Display.getCurrent().getSystemColor(SWT.COLOR_WIDGET_BACKGROUND);

    /**
     * Folder containing the layers of the Atlas.
     */
    private TabFolder folder;

    /**
     * Constructor.
     */
    public AtlasBuilder() {
        // Nothing to do.
    }

    /**
     * @{inherit-doc
     */
    @Override
    public void createPartControl(Composite parent) {
        final Composite container = new Composite(parent, SWT.NONE);
        container.setLayout(new FillLayout());
        folder = new TabFolder(container, SWT.BOTTOM);
        folder.setLayout(new FillLayout());
    }

    /**
     * @{inherit-doc
     */
    @Override
    public void setFocus() {
        // Nothing to do.
    }

    /**
     * Query to rebuild the Atlas.
     */
    public void rebuildAtlas() {
        Atlas atlas = Atlas.getDefault();
        atlas.rebuild(); // This operation is synchronous.
        final List<Atlas.Layer> layers = new LinkedList<Atlas.Layer>();
        atlas.fetchLayers(layers);
        getViewSite().getShell().getDisplay().syncExec(new Runnable() {
            @Override
            public void run() {
                refreshTabs(layers);
            }
        });
    }

    /**
     * Refresh tabs with Atlas Layers.
     * @param layers Layer of the Atlas.
     */
    protected void refreshTabs(final List<Atlas.Layer> layers) {
        TabItem[] tabs = folder.getItems();
        // Kill/Dispose former tabs.
        for (TabItem i : tabs) {
            i.dispose();
        }

        // Create a tab per layer.
        int size = layers.size();
        final Map<Sprite, Point> sprites = new HashMap<Sprite, Point>();
        for (int i = 0; i < size; ++i) {
            final Atlas.Layer layer = layers.get(i);
            final TabItem item = new TabItem(folder, SWT.NONE);
            item.setText(String.format("Layer %d", Integer.valueOf(i))); //$NON-NLS-1$
            final Canvas canvas = new Canvas(folder, SWT.V_SCROLL | SWT.H_SCROLL | SWT.NO_BACKGROUND);
            canvas.setSize(Atlas.DEFAULT_SIZE, Atlas.DEFAULT_SIZE);
            item.setControl(canvas);
            canvas.addPaintListener(new PaintListener() {
                @Override
                public void paintControl(PaintEvent e) {
                    layer.fetchComposition(sprites);
                    GC gc = e.gc;
                    int oX = canvas.getHorizontalBar().getSelection();
                    int oY = canvas.getVerticalBar().getSelection();
                    gc.setForeground(BACKGROUND);
                    gc.fillRectangle(0, 0, e.width, e.height);
                    for (Map.Entry<Sprite, Point> i : sprites.entrySet()) {
                        Sprite sprite = i.getKey();
                        Point position = i.getValue();
                        Rectangle bds = sprite.getBounds();
                        gc.drawImage(sprite.getImage(), bds.x, bds.y, bds.width, bds.height, position.x - oX,
                                position.y - oY, bds.width, bds.height);
                    }
                }
            });
            ScrollBar scrollbar = canvas.getVerticalBar();
            scrollbar.setMinimum(0);
            scrollbar.setMaximum(Atlas.DEFAULT_SIZE);
            scrollbar.addListener(SWT.Selection, new Listener() {
                @Override
                public void handleEvent(Event event) {
                    canvas.redraw();
                }
            });
            scrollbar = canvas.getHorizontalBar();
            scrollbar.setMinimum(0);
            scrollbar.setMaximum(Atlas.DEFAULT_SIZE);
            scrollbar.addListener(SWT.Selection, new Listener() {
                @Override
                public void handleEvent(Event event) {
                    canvas.redraw();
                }
            });
            item.addListener(SWT.Resize, new Listener() {
                @Override
                public void handleEvent(Event event) {
                    canvas.redraw();
                }
            });
        }
    }
}
