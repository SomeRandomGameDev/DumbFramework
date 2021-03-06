package org.dumbframework.tools.atlasmaker;

import java.util.HashMap;
import java.util.Map;

import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Image;
import org.eclipse.ui.plugin.AbstractUIPlugin;
import org.osgi.framework.BundleContext;

/**
 * The activator class controls the plug-in life cycle
 */
public class Activator extends AbstractUIPlugin {

    /**
     * Plugin identifier.
     */
    public static final String PLUGIN_ID = "org.dumbframework.tools.atlasmaker"; //$NON-NLS-1$

    /**
     * Plugin instance.
     */
    private static Activator plugin;

    /**
     * Loaded images.
     */
    private Map<String, Image> loadedImages = new HashMap<String, Image>();

    /**
     * The constructor.
     */
    public Activator() {
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void start(BundleContext context) throws Exception {
        super.start(context);
        plugin = this;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void stop(BundleContext context) throws Exception {
        plugin = null;
        for (Image image : loadedImages.values()) {
            image.dispose();
        }
        loadedImages.clear();
        super.stop(context);
    }

    /**
     * Returns the shared instance
     * @return the shared instance
     */
    public static Activator getDefault() {
        return plugin;
    }

    /**
     * Returns an image descriptor for the image file at the given plug-in relative path
     * @param path the path
     * @return the image descriptor
     */
    public static ImageDescriptor getImageDescriptor(String path) {
        return imageDescriptorFromPlugin(PLUGIN_ID, path);
    }

    /**
     * Provide access to image loaded from the bundle.
     * @param path Bundle-wise path.
     * @return An image or null if the path is incorrect.
     */
    public Image getBundleImage(String path) {
        Image result = loadedImages.get(path);
        if (null == result) {
            ImageDescriptor descriptor = getImageDescriptor(path);
            if (descriptor != null) {
                result = descriptor.createImage();
                loadedImages.put(path, result);
            } else {
                result = plugin.getWorkbench().getDisplay().getSystemImage(SWT.ICON_ERROR);
            }
        }
        return result;
    }
}
