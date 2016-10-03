<?php

/**
 * @file
 * EUAID Devel module drush integration.
 */

/**
 * Implements hook_drush_command().
 *
 * Returns an associative array describing your command(s).
 *
 * @see drush_parse_command()
 */
function euaid_devel_drush_command() {
  $items = array();
  $items['euaid-manipulate-fields'] = array(
    'description' => "Lock/Unlock the content type fields.",
    'aliases' => array('euaid-mf'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function euaid_devel_drush_help($section) {
  switch ($section) {
    case 'drush:euaid-manipulate-fields':
      return dt("Lock/Unlock the content type fields");
  }
}

/**
 * Callback for the euaid-lock-fields command.
 */
function drush_euaid_devel_euaid_manipulate_fields() {
  $types = array();
  $options = array(
    'node' => 'Node',
    'profile2' => 'Profile2',
  );
  $entity_choice = drush_choice($options, dt('What entity type do you want to manipulate?'));
  if ($entity_choice == 'node') {
    $types = node_type_get_types();
  }
  elseif ($entity_choice == 'profile2') {
    $types = profile2_get_types();
  }

  $options = array(
    'lock' => 'Lock fields',
    'unlock' => 'Unlock fields',
  );
  $choice = drush_choice($options, dt('What do you want to do?'));
  if ($choice == 'lock') {
    $lock = 1;
    $unlock = 0;
  }
  else {
    $lock = 0;
    $unlock = 1;
  }

  if (!empty($types)) {
    foreach ($types as $type_name => $type_object) {
      if (strpos($type_name, 'euaid_') !== FALSE) {
        $fields = field_info_instances($entity_choice, $type_name);
        foreach ($fields as $field_name => $field_info_array) {
          $field = field_read_field($field_name);
          if ($field['locked'] == $unlock) {
            // Means 0: unlock; 1: lock.
            $field['locked'] = $lock;
            field_update_field($field);
          }
        }
      }
    }
  }
}
