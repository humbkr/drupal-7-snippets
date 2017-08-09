<?php
/**
 * @file
 * Create or update a path alias.
 *
 * Dependencies:
 *   - path
 */

/**
 * Creates or updates a path alias.
 *
 * Note: This function will replace an existing path alias, not create a new
 * one alongside the previous one.
 *
 * @param string $source
 *   The original path.
 * @param string $alias
 *   The alias to create or update.
 * @param string $language
 *   Language of the alias.
 */
function MODULE_add_path_alias($source, $alias, $language) {
  $conditions = array(
    'source' => $source,
    'language' => $language,
  );
  $path = path_load($conditions);
  if (!$path) {
    $path = $conditions;
  }
  $path['alias'] = $alias;
  path_save($path);
}