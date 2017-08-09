<?php
/**
 * @file
 * Add all source strings (t('some string')) from source code into database.
 *
 * This is very useful when working with import files to initialize a new Drupal
 * site.
 *
 * Use case example:
 *
 * You created a Drupal distribution or a master Drupal site for a brand that
 * needs to be deployed in several countries. For each countries, the local
 * translators team give you all translations (excel / csv file for example).
 *
 * If you were to import these translations just after import, it wouldn't
 * work because Drupal would not found the source strings in the database.
 * To do that you would have to visit all the site's pages so Drupal adds each
 * source string to the database (similar to the problem of strings not
 * appearing in the "translate interface" admin page until you visited at least
 * once the page where the string is located).
 *
 * This function allows you to scan a source code directory to extract all
 * the source strings, before adding the translations.
 *
 * Dependencies:
 *   - potx
 */

/**
 * Parse directory and add translatable strings from source code into database.
 *
 * This function is a mix of different functions from potx module.
 *
 * @param string $directory
 *   The directory to parse.
 */
function MODULE_insert_all_translatable_strings_from_source_code($directory) {
  module_load_include('inc', 'potx');
  module_load_include('local.inc', 'potx');

  potx_local_init($directory);
  $files = _potx_explore_dir($directory . '/');
  $strip_prefix = 1 + strlen($directory);

  foreach ($files as $file) {
    _potx_process_file($file, $strip_prefix);
  }
  potx_finish_processing('_potx_save_string');

  // Get strings and versions by reference.
  $strings  = _potx_save_string(NULL, NULL, NULL, 0, POTX_STRING_RUNTIME);
  $versions = _potx_save_version();

  // We might not have any string recorded in this string mode.
  if (!is_array($strings)) {
    return;
  }

  foreach ($strings as $string => $string_info) {
    foreach ($string_info as $context => $file_info) {
      // Build a compact list of files this string occured in.
      $occured = $file_list = array();

      // Look for strings appearing in multiple directories (ie.
      // different subprojects). So we can include them in general.pot.
      foreach ($file_info as $file => $lines) {
        $occured[] = "$file:" . implode(';', $lines);
        if (isset($versions[$file])) {
          $file_list[] = $versions[$file];
        }
      }

      // Mark duplicate strings (both translated in
      // the app and in the installer).
      $comment = implode(" ", $occured);
      if (strpos($comment, '(dup)') !== FALSE) {
        $comment = '(duplicate) ' . str_replace('(dup)', '', $comment);
      }

      db_merge('locales_source')
        ->insertFields(array(
          'location' => $comment,
          'version' => VERSION,
        ))
        ->key(array(
          'source' => $string,
          'context' => (string) $context,
          'textgroup' => 'default',
        ))
        ->execute();
    }
  }
}