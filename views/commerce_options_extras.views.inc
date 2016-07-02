<?php
/**
 * Created by PhpStorm.
 * User: Matt
 * Date: 1/27/2016
 * Time: 5:55 AM
 */
/**
 * Implements hook_views_data_alter().
 */
function commerce_options_extras_views_data_alter(&$data) {
  $data['commerce_line_item']['product_options']  = array(
    'title' => 'Selected Product Options',
    'real field' => 'line_item_id',
    'field' => array(
      'handler' => 'commerce_options_extras_handler_field_options',
    ),
  );
}