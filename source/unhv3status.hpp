/*Copyright (C) 2014  별님

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef UNHV3STATUS_HPP
#define UNHV3STATUS_HPP

/** unhv3 작업 상태에 대한 정보 열거자.
  */
enum class Unhv3Status
{
    NO_ERROR,        ///< 오류 없음
    FILE_NOT_EXIST,  ///< 파일 존재하지 않음
    CANT_READ_FILE,  ///< 파일을 읽을수 없음
    IS_NOT_FILE,     ///< 파일이 아님
    NOT_HV3_FORMAT   ///< hv3 포멧이 아님
};

#endif // UNHV3STATUS_HPP
