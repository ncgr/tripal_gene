<?php

/**
 *  @file
 *  This file contains the basic functions for views integration of
 *  chado/tripal stock tables. Supplementary functions can be found in
 *  ./views/
 *
 *  Documentation on views integration can be found at 
 *  http://views2.logrus.com/doc/html/index.html.
 */
 
/*************************************************************************
 * Implements hook_views_data()
 * Purpose: Describe chado/tripal tables & fields to views
 *
 * @return: a data array which follows the structure outlined in the
 *   views2 documentation for this hook. Essentially, it's an array of table
 *   definitions keyed by chado/tripal table name. Each table definition 
 *   includes basic details about the table, fields in that table and
 *   relationships between that table and others (joins)
 */
require_once('views/stock.views.inc');
function tripal_stock_views_data()  {
  $data = array();

  $data = array_merge($data, retrieve_stock_views_data());
  
  return $data;
}

/*************************************************************************
 * Implements hook_views_handlers()
 * Purpose: Register all custom handlers with views
 *   where a handler describes either "the type of field", 
 *   "how a field should be filtered", "how a field should be sorted"
 *
 * @return: An array of handler definitions
 */
function tripal_stock_views_handlers() {
 return array(
   'info' => array(
     'path' => drupal_get_path('module', 'tripal_stock') . '/views/handlers',
   ),
   'handlers' => array(
     'views_handler_field_computed_nid' => array(
       'parent' => 'views_handler_field_numeric',
     ),
     'views_handler_field_stockprop_by_type' => array(
       'parent' => 'views_handler_field_prerender_list',
     ),
		'views_handler_field_stockprop_all' => array(
       'parent' => 'views_handler_field_prerender_list',
     ),
		'views_handler_field_stockrel_by_type' => array(
       'parent' => 'views_handler_field_prerender_list',
     ),
		'views_handler_field_stockrel_all' => array(
       'parent' => 'views_handler_field_prerender_list',
     ),
		'views_handler_field_stock_dbxref_by_type' => array(
       'parent' => 'views_handler_field_prerender_list',
     ),
		'views_handler_field_stock_dbxref_all' => array(
       'parent' => 'views_handler_field_prerender_list',
     ),
   ),
 );
}

/**
 * Implements hook_views_pre_render
 * Purpose: Intercepts the view after the query has been executed
 *   All the results are stored in $view->result
 *   Looking up the NID here ensures the query is only executed once
 *   for all stocks in the table.
 *
 * @todo add if !<chado/drupal same db> around NID portion
 */
function tripal_stock_views_pre_render	(&$view) {
	if (preg_match('/stock/', $view->base_table)) {
		
		// retrieve the stock_id for each record in the views current page
		$stock_ids = array();
		foreach ($view->result as $row_num => $row) {
			$stock_ids[$row_num] = $row->stock_id;
		}

		// Using the list of stock_ids from the view
		// lookup the NIDs from drupal
		// and add that to the results of the view
		$sql = "SELECT nid, stock_id FROM chado_stock WHERE stock_id IN (".implode(',',$stock_ids).")";
		$resource = db_query($sql);
		while ($r = db_fetch_object($resource)) {
			$key = array_search($r->stock_id, $stock_ids);
			$view->result[$key]->nid = $r->nid;
		}
		
		//-----Properties------------------------------------------
		$field_names = array_keys($view->field);
		//if any property fields are in the current view
		$property_field_names = preg_grep('/properties/',$field_names);
		if (!empty($property_field_names)) {
			$sql = "SELECT stockprop.*, cvterm.name as type_name FROM stockprop "
				."INNER JOIN cvterm cvterm ON stockprop.type_id=cvterm.cvterm_id "
				."WHERE stockprop.stock_id IN (".implode(',',$stock_ids).")";
			$previous_db = db_set_active('chado');
			$resource = db_query($sql);
			db_set_active($previous_db);
		
			$view->result[$key]->properties = array();
			while ($r = db_fetch_object($resource)) {
				$key = array_search($r->stock_id, $stock_ids);
				$view->result[$key]->properties[] = $r;
			}
		}
		
		//-----Relationships----------------------------------------
		//if any relationship fields are in the current view
		$relationship_field_names = preg_grep('/relationships/', $field_names);
		if (!empty($relationship_field_names)) {
			$sql = "SELECT stock_relationship.*, cvterm.name as type_name, "
				."subject_stock.name as subject_name, object_stock.name as object_name "
				."FROM stock_relationship "
				."LEFT JOIN stock subject_stock ON stock_relationship.subject_id=subject_stock.stock_id "
				."LEFT JOIN stock object_stock ON stock_relationship.object_id=object_stock.stock_id "
				."LEFT JOIN cvterm cvterm ON stock_relationship.type_id = cvterm.cvterm_id "
				."WHERE stock_relationship.subject_id IN (".implode(',',$stock_ids).") "
				."OR stock_relationship.object_id IN (".implode(',',$stock_ids).") ";
			$previous_db = db_set_active('chado');
			$resource = db_query($sql);
			db_set_active($previous_db);
			
			while ($r = db_fetch_object($resource)) {
				if (in_array($r->subject_id, $stock_ids)) {
					$key = array_search($r->subject_id, $stock_ids);
					$r->stock_id = $r->subject_id;
					$view->result[$key]->relationships[] = clone $r;
				}
				if (in_array($r->object_id, $stock_ids)) {
					$key = array_search($r->object_id, $stock_ids);
					$r->stock_id = $r->object_id;
					$view->result[$key]->relationships[] = clone $r;
				}
			}
		}
		
		//-----DB References--------------------------------------------
		//if any dbxref fields are in the current view
		$dbxref_field_names = preg_grep('/dbxref/',$field_names);
		if (!empty($dbxref_field_names)) {
			$sql = "SELECT stock_dbxref.*, dbxref.db_id, db.name as db_name, db.urlprefix, "
				."dbxref.accession, dbxref.version, dbxref.description "
				."FROM stock_dbxref "
				."LEFT JOIN dbxref dbxref ON stock_dbxref.dbxref_id=dbxref.dbxref_id "
				."LEFT JOIN db db ON dbxref.db_id=db.db_id "
				."WHERE stock_dbxref.stock_id IN (".implode(',',$stock_ids).")";
			$previous_db = db_set_active('chado');
			$resource = db_query($sql);
			db_set_active($previous_db);
		
			$view->result[$key]->dbxref = array();
			while ($r = db_fetch_object($resource)) {
				$key = array_search($r->stock_id, $stock_ids);
				$view->result[$key]->dbxref[] = $r;
			}
		}
		
	} //if we're dealing with a stock view
}