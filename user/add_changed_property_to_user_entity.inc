<?php
/**
 * @file
 * Add "changed" property to user entity to be able to track modifications.
 *
 * Dependencies:
 *   - user
 */

/* Code to put in .install file. */

/**
 * Implements hook_install().
 */
function MODULE_install() {
  // Add a "changed" property to user profile.
  $field = array(
    'type' => 'int',
    'not null' => TRUE,
    'default' => 0,
    'description' => 'Timestamp for last time user was updated.',
  );
  $index = array(
    'indexes' => array(
      'changed' => array('changed')
    ),
  );
  db_add_field('users', 'changed', $field, $index);
}

/**
 * Implements hook_uninstall().
 */
function MODULE_uninstall() {
  db_drop_field('users', 'changed');
}

/**
 * Implements hook_schema_alter().
 */
function MODULE_schema_alter(&$schema) {
  $schema['users']['fields']['changed'] = array(
    'type' => 'int',
    'not null' => TRUE,
    'default' => 0,
    'description' => 'Timestamp for last time user was updated.',
  );
}

/* Code to put in .module file. */

/**
 * Implements hook_entity_property_info_alter().
 */
function MODULE_entity_property_info_alter(&$info) {
  $properties = &$info['user']['properties'];
  $properties['changed'] = array(
    'label' => t('Modification date'),
    'description' => t('The date of the user profile last modification.'),
    'type' => 'date',
    'getter callback' => 'MODULE_entity_property_getter',
  );
}

/**
 * Modification date getter for hook_entity_property_info_alter()
 *
 * @param $entity
 *   The entity object.
 *
 * @return int
 *   The modification date as a timestamp get from the entity.
 */
function MODULE_entity_property_getter($entity) {
  return $entity->changed;
}

/**
 * Implements hook_user_update().
 */
function MODULE_user_presave(&$edit, $account, $category) {
  $date_time = new DateTime();
  $account->changed = $date_time->getTimestamp();
}
