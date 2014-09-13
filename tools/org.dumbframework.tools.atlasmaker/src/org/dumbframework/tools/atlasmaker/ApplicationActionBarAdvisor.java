package org.dumbframework.tools.atlasmaker;

import org.eclipse.jface.action.GroupMarker;
import org.eclipse.jface.action.IMenuManager;
import org.eclipse.jface.action.MenuManager;
import org.eclipse.jface.action.Separator;
import org.eclipse.ui.IWorkbenchActionConstants;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.actions.ActionFactory;
import org.eclipse.ui.actions.ActionFactory.IWorkbenchAction;
import org.eclipse.ui.application.ActionBarAdvisor;
import org.eclipse.ui.application.IActionBarConfigurer;

/**
 * An action bar advisor is responsible for creating, adding, and disposing of
 * the actions added to a workbench window. Each window will be populated with
 * new actions.
 */
public class ApplicationActionBarAdvisor extends ActionBarAdvisor {

    private static final String SESSION_INSERTION = "sessionInsertion"; //$NON-NLS-1$

    private static final String PROFILE_INSERTION = "profileInsertion"; //$NON-NLS-1$

    private static final String COMMON_INSERTION  = "commonInsertion"; //$NON-NLS-1$

    // Actions - important to allocate these only in makeActions, and then use
    // them
    // in the fill methods. This ensures that the actions aren't recreated
    // when fillActionBars is called with FILL_PROXY.
    private IWorkbenchAction    exitAction;
    private IWorkbenchAction    aboutAction;

    /**
     * Default Constructor.
     * @param configurer
     */
    public ApplicationActionBarAdvisor(IActionBarConfigurer configurer) {
        super(configurer);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    protected void makeActions(final IWorkbenchWindow window) {
        // Creates the actions and registers them.
        // Registering is needed to ensure that key bindings work.
        // The corresponding commands keybindings are defined in the plugin.xml
        // file.
        // Registering also provides automatic disposal of the actions when
        // the window is closed.

        exitAction = ActionFactory.QUIT.create(window);
        register(exitAction);

        aboutAction = ActionFactory.ABOUT.create(window);
        register(aboutAction);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    protected void fillMenuBar(IMenuManager menuBar) {
        MenuManager menu = new MenuManager("File", //$NON-NLS-1$
                IWorkbenchActionConstants.M_FILE);
        menuBar.add(menu);

        menu.add(new GroupMarker(COMMON_INSERTION));

        menu.add(new Separator());

        menu.add(new GroupMarker(PROFILE_INSERTION));

        menu.add(new Separator());

        menu.add(new GroupMarker(SESSION_INSERTION));

        menu.add(new Separator());

        menu.add(exitAction);

        menu = new MenuManager("Help", //$NON-NLS-1$
                IWorkbenchActionConstants.M_HELP);
        menuBar.add(menu);
        menu.add(aboutAction);
    }

}
