<?php
/**
 * @file
 * Manually does the job of t() by inserting a new translation for a
 * string in the database.
 */

/**
 * Translate a string.
 *
 * Manually does the job of t() by adding a new translation for a
 * string.
 * If the original string is not found, insert it.
 *
 * @param string $source
 *   The source string.
 * @param string $target_string
 *   The translation string.
 * @param string $language
 *   The lanquage of the translated string.
 * @param string $context
 *   The translation context if any.
 */
function MODULE_add_string_translation($source, $target_string, $language, $context = NULL) {
  if (empty($target_string)) {
    return;
  }

  $db_source = db_select('locales_source', 'ls');
  $db_source->fields('ls', array('lid'));
  $db_source->condition('ls.source', $source);
  if ($context) {
    $db_source->condition('ls.context', $context);
  }
  $source_id = $db_source->execute()->fetchField();

  if ($source_id) {
    $db_target = db_select('locales_target', 'lt');
    $db_target->fields('lt', array('lid'));
    $db_target->condition('lt.lid', $source_id);
    $db_target->condition('lt.language', $language);
    $target = $db_target->execute()->fetchField();

    if ($target !== FALSE) {
      $num_updated = db_update('locales_target');
      $num_updated->fields(array(
        'translation' => $target_string,
      ));
      $num_updated->condition('lid', $source_id);
      $num_updated->condition('language', $language);
      $num_updated->execute();
    }
    else {
      $fields = array(
        'lid' => $source_id,
        'translation' => $target_string,
        'language' => $language,
      );
      $insert = db_insert('locales_target');
      $insert->fields($fields);
      $insert->execute();
    }
  }
  else {
    $context_id = ($context) ? $context : '<no context>';
    drupal_set_message(t('Translation source string !string not found for context !context', array('!string' => $source, '!context' => $context_id), array('context' => 'danonino_back')));
  }
}