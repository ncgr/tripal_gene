<?php

/*************************************************************************
 * @file: THIS IS A TEMPLATE AND SHOULD NOT BE INCLUDED IN THE MODULE CODE
 *
 *   - Every instance of nd_reagentprop should be replaced with the name of your table
 *   - If this is a base table (you want a view where every row is a row from this table)
 *     then change $data['nd_reagentprop']['table'] to $data['nd_reagentprop']['table']['base'] 
 *     and $data['nd_reagentprop']['table']['database'] to $data['nd_reagentprop']['table']['base']['database']
 *   - Relationships between this table and others: YYY is the table you are trying to join to this
 *     one. You want to join a table to this one if this table contains a foreign key to the other
 *     table. If the join between this table and another is through a linking table
 *     (ie: library-nd_reagentprop/YYY => library_feature-XY => feature-nd_reagentprop/YYY) then make the join in both
 *     directions (ie: in the file nd_reagentprop.views.inc and the file YYY.views.inc
 *   - Create a field definition for each field in this table using the example fields already
 *     listed. Match the type of the database field to the field definition listed below.
 *     (ie: for a text/varchar field from the database use plain_text_field below)
 * 
 *  NOTE: Creating the table definition file is not enough. You also need to call the 
 *        retrieve_nd_reagentprop_views_data() function from ../tripal_natural_diversity.views.inc:tripal_natural_diversity_views_data()
 *        by adding the following line:
 *           $data = array_merge($data, retrieve_nd_reagentprop_views_data());
 *        to the function and including the file directly above the function (blow the function 
 *        header by adding:
 *           require_once('views/nd_reagentprop.views.inc');
 *
 *  REMOVE THIS COMMENT IN THE COPY!
 */ 
 
/**
 *  @file
 *  This file defines the data array for a given chado table. This array
 *  is merged into a larger array containing definitions of all tables associated
 *  with this module in:
 *  @see tripal_natural_diversity.views.inc --in tripal_natural_diversity_views_data()
 *
 *  Documentation on views integration can be found at 
 *  http://views2.logrus.com/doc/html/index.html.
 */

/*************************************************************************
 * Purpose: this function returns the portion of the data array 
 *   which describes the nd_reagentprop table, it's fields and any joins between it and other tables
 * @see tripal_natural_diversity_views_data() --in tripal_natural_diversity.views.inc
 *
 * Table: nd_reagentprop
 * @code
 * nd_reagentprop-Copy/Paste Table SQL code here-nd_reagentprop
 * @endcode
 */
 function retrieve_nd_reagentprop_views_data() {
  global $db_url;
  $data = array();

  // if the chado database is not local to the drupal database
  // then we need to set the database name.  This should always
  // be 'chado'.
  if(is_array($db_url) and array_key_exists('chado',$db_url)){
    $database = 'chado';
  }
   
  //Basic table definition-----------------------------------
  $data['nd_reagentprop']['table']['group'] = t('Chado nd_reagentprop');
  
  $data['nd_reagentprop']['table'] = array(
    'field' => 'primary_id',
    'title' => t('Chado nd_reagentprop'),
    'help' => t('Enter some user-friendly description of this tables purpose to the user.'),
  );
  if($database){
     $data['nd_reagentprop']['table']['database'] = $database;
  }

  
  //Relationship Definitions---------------------------------
  //Join: nd_reagentprop => YYY
  $data['nd_reagentprop']['table']['join']['YYY'] = array(
    'left_field' => 'primary key in YYY table',
    'field' => 'foreign key in nd_reagentprop table',
  );  
  
  //Join: nd_reagentprop => XY => YYY
  $data['nd_reagentprop']['table']['join']['XY'] = array(
    'left_field' => 'matching nd_reagentprop key in the XY table',
    'field' => 'foreign key in nd_reagentprop table',
  );  
  $data['nd_reagentprop']['table']['join']['YYY'] => array(
    'left_table' => 'XY',
    'left_field' => 'matching nd_reagentprop key in the XY table',
    'field' => 'foreign key in nd_reagentprop table',
  );
  $data['XY']['table']['join']['YYY'] = array(
    'left_field' => 'foreign key in YYY table',
    'field' => 'matching YYY key in the XY table',
  );
   
  //Table Field Definitions----------------------------------
      
  //Field: nd_reagentprop_id (primary key)
  $data['nd_reagentprop']['field_name'] = array(
    'title' => t('nd_reagentprop Primary Key'),
    'help' => t('A unique index for every nd_reagentprop.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  /*.......................................................
   * Beginning of Example Field definitions
   * Remove this section when done
   */

  //Field: plain_text_field (chado datatype)   
  $data['nd_reagentprop']['plain_text_field'] = array(
    'title' => t('Human-Readable Name'),
    'help' => t('Description of this field.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  //Field: numeric_field (chado datatype)   
  $data['nd_reagentprop']['numeric_field'] = array(
    'title' => t('Human-Readable Name'),
    'help' => t('Description of this field.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  //Field: boolean_field (chado datatype)   
  $data['nd_reagentprop']['boolean_field'] = array(
    'title' => t('Human-Readable Name'),
    'help' => t('Description of this field.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
  );

  //Field: unix_timestamp (chado datatype)   
  $data['nd_reagentprop']['unix_timestamp'] = array(
    'title' => t('Human-Readable Name'),
    'help' => t('Description of this field.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  //Field: human_readable_date (chado datatype)   
  $data['nd_reagentprop']['human_readable_date'] = array(
    'title' => t('Human-Readable Name'),
    'help' => t('Description of this field.'),
    'field' => array(
      'handler' => 'views_handler_field_readble_date',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );
   
   /*
    * End of Example Field definitions
    *......................................................./
    
  return $data;
}