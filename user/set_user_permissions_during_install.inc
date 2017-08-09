<?php
/**
 * @file
 * Helper to set user permissions during install.
 *
 * Dependencies:
 *   - user
 */

/**
 * Custom user_role_grant_permissions function that can run during install.
 *
 * Does the same thing as user_role_grant_permissions
 * but it can run at install time.
 *
 * Example:
 * $permissions = array(
 *   'i18n string' => array('translate user-defined strings'),
 *   'locale' => array('translate interface'),
 * );
 * MODULE_grant_permissions(DRUPAL_AUTHENTICATED_RID, $permissions);
 *
 * @see user_role_grant_permissions
 *
 * @param int $rid
 *   The role ID for which to set permissions.
 * @param array $permissions
 *   The permissions to set.
 */
function MODULE_grant_permissions($rid, array $permissions = array()) {
  // This line doesn't work on install / enable hooks, that's why we need
  // the custom function:
  // $modules = user_permission_get_modules();

  // Grant new permissions for the role.
  foreach ($permissions as $module => $permission_list) {
    foreach ($permission_list as $name) {
      db_merge('role_permission')
        ->key(array(
          'rid' => $rid,
          'permission' => $name,
        ))
        ->fields(array(
          'module' => $module,
        ))
        ->execute();
    }
  }

  // Clear the user access cache.
  drupal_static_reset('user_access');
  drupal_static_reset('user_role_permissions');
}
