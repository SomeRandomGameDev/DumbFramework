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

import org.dumbframework.tools.atlasmaker.model.Atlas;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.TabFolder;
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
        Atlas.getDefault().rebuild(); // This operation is synchronous.
        // TODO Redraw.
    }
}
