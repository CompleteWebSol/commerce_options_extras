<?php
/**
 * Created by PhpStorm.
 * User: Matt
 * Date: 1/27/2016
 * Time: 5:59 AM
 */

/**
 * Field handler to provide simple renderer that displays selected options
 *
 * @ingroup views_field_handlers
 */
class commerce_options_extras_handler_field_options extends views_handler_field {
  /**
   * Constructor to provide additional field to add.
   *
   * This constructer assumes the taxonomy_term_data table. If using another
   * table, we'll need to be more specific.
   */
  function construct() {
    parent::construct();
    /*
    $this->additional_fields['vid'] = 'vid';
    $this->additional_fields['tid'] = 'tid';
    $this->additional_fields['vocabulary_machine_name'] = array(
      'table' => 'taxonomy_vocabulary',
      'field' => 'machine_name',
    );
    */
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_facet'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  /**
   * Provide link to taxonomy option
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_facet'] = array(
      '#title' => t('Link this field to its taxonomy search facet'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_facet']),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_list($data, $values) {
    // Due to the way DH setup options, we're going to have to pull the product to see
    // which options are actually enabled
    $sku = $values->_field_data['commerce_line_item_field_data_commerce_line_items_line_item_']['entity']->line_item_label;
    $product = commerce_product_load_by_sku($sku);
    // If no options, then we're done.
//    dpm($product,'product');
    // find pricing attributes fields
    /*
    if(!isset($product->field_options) || empty($product->field_options)) {
      return null;
    }
    */
    $options = commerce_option_load_by_line_item($data);

    if(empty($options)) {
      return null;
    }
    $fields = array();
    // lets find the fields
    foreach($options as $optid=>$option) {

//    dpm($options,'options');
      $info = field_info_instances('commerce_option', $option->set_id);
//    $info = field_read_fields(array('type'=>'commerce_pricing_attributes','bundle'=>$product->type));
//    dpm($info,'info');
      $option_fields = array();
      /*
      foreach($info as $key=>$value) {
        if(isset($options->{$key}[LANGUAGE_NONE])) {
          if(!isset($option_fields[$key])) {
            $option_fields[$key] = field_info_field($key);
          }
          $option_value = $options->{$key}[LANGUAGE_NONE][0]['value'];
          $fields[] = array(
            'data' => $info[$key]['label'] . ':' . $option_fields[$key]['settings']['allowed_values'][$option_value],
          );
        }

      }
      */
      // kinda heinky but iterate over selected options and get their label/values
//    foreach($product->field_options[LANGUAGE_NONE] as $key=>$value) {
      foreach ($product->{$option->field_name}[LANGUAGE_NONE] as $key => $value) {
//      dpm($value['set_details'],'Set Details');
        foreach ($value['set_details'] as $k => $v) {
          if (!isset($option_fields[$k])) {
            $option_fields[$k] = field_info_field($k);
          }
          if (!isset($option->{$k}[LANGUAGE_NONE][0]['value'])) {
            continue;
          }
          $option_value = $option->{$k}[LANGUAGE_NONE][0]['value'];
          if($info[$k]['widget']['module'] == 'text') {
            $fields[] = array(
              'data' => $info[$k]['label'] . ': ' .$option_value,
            );
          } else {
            $fields[] = array(
              'data' => $info[$k]['label'] . ': ' . $option_fields[$k]['settings']['allowed_values'][$option_value],
            );
          }
        }
      }
    }
    if(empty($fields)) {
      return null;
    }
    $data = theme_item_list(array(
      'title' => '',
      'items' => $fields,
      'type' => 'ul',
      'attributes' => array(),
    ));
    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_list($this->sanitize_value($value), $values);
  }
}
