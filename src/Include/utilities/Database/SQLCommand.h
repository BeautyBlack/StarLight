/*
 *****************************************************************
 *                 C O P Y R I G H T  (c) 2020
 *                    All Rights Reserved
 *****************************************************************
 *
 *       THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF
 *                      Zhang Jiaqi.
 *      The copyright notice above does not evidence any
 *     actual or intended publication of such source code.
 *
 *****************************************************************
 *
 *  Author: Zhang Jiaqi
 *
 */

#ifndef SQL_COMMAND_835E816A6A254B0E8A64AE73335E3ABD
#define SQL_COMMAND_835E816A6A254B0E8A64AE73335E3ABD

#pragma once

static LPCTSTR STR_DELETE_STUDY_COMMAND = "UPDATE Study SET %s=1 where %s=%s;";

static LPCTSTR STR_QUERY_STUDY_COMMAND = "SELECT * FROM Study WHERE Deleted=0";

static LPCTSTR STR_INSERT_STUDY_COMMAND = "INSERT INTO Study ([%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]) VALUES(%d, %d,'%s','%s',%d,'%s','%s','%s','%s','%s',%d,'%s','%s','%s',%d);";

static LPCTSTR STR_IS_STUDY_EXIST_COMMAND = "SELECT TOP 1 * FROM Study WHERE %s='%s';";

#endif
