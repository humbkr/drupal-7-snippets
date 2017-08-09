<?php
/**
 * @file
 * Display translated taxonomy terms in views.
 *
 * Dependencies:
 *   - views
 *   - i18n
 */

/**
 * Implements hook_views_api().
 */
function MODULE_views_api() {
  return array(
    'api' => 3,
  );
}

/**
 * Implements hook_views_pre_render().
 *
 * Allows taxonomy terms translation in views to work.
 */
function MODULE_views_pre_render(&$view) {
  if ($view->base_table == 'taxonomy_term_data') {
    global $language;

    foreach ($view->result as $delta => $term ){
      if (isset($term->tid)) {
        i18n_string_translate_langcode($language->language);

        $localized_term = i18n_taxonomy_localize_terms(taxonomy_term_load($term->tid));
        $term->tid = $localized_term->tid;
        $term->taxonomy_term_data_name = $localized_term->name;
        $term->taxonomy_term_data_description = $localized_term->description;
      }
    }
  }
}
