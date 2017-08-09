<?php
/**
 * @file
 * Add active trail indicators into menu_tree_all_data menu tree.
 *
 * Dependencies:
 *   - menu
 */

/**
 * Add active trail indicators into menu_tree_all_data menu tree.
 *
 * The data returned by menu_tree_page_data() has link['in_active_trail'] set
 * to TRUE for each menu item in the active trail. The data returned from
 * menu_tree_all_data() does not contain the active trail indicators.
 * This is a helper function that adds it back in.
 *
 * Usage:
 * $menu_tree = menu_tree_all_data('main-menu');
 * $menu_tree_with_active_trail = MODULE_menu_tree_add_active_path($menu_tree);
 * $variables['main_menu'] = menu_tree_output($menu_tree_with_active_trail);
 *
 * @param array $tree
 *   The menu tree.
 */
function MODULE_menu_tree_add_active_path(&$tree) {
  // Grab any menu item to find the menu_name for this tree.
  $menu_item = current($tree);
  $tree_with_trail = menu_tree_page_data($menu_item['link']['menu_name']);

  // To traverse the original tree down the active trail, we use a pointer.
  $subtree_pointer = &$tree;

  // Find each key in the active trail.
  while ($tree_with_trail) {
    foreach ($tree_with_trail as $key => &$value) {
      if ($tree_with_trail[$key]['link']['in_active_trail']) {
        // Set the active trail info in the original tree.
        $subtree_pointer[$key]['link']['in_active_trail'] = TRUE;
        // Continue in the subtree, if it exists.
        $tree_with_trail = &$tree_with_trail[$key]['below'];
        $subtree_pointer = &$subtree_pointer[$key]['below'];
        break;
      }
      else {
        unset($tree_with_trail[$key]);
      }
    }
  }
}
