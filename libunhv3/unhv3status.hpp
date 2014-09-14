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

#include <QString>

/** unhv3 작업 상태.
  */
class Unhv3Status
{
public:
    enum Status {
        NO_ERROR,           ///< 오류 없음
        FILE_NOT_EXIST,     ///< 파일 존재하지 않음
        CANT_READ_FILE,     ///< 파일을 읽을수 없음
        NOT_HV3_FORMAT,     ///< hv3 포멧이 아님
        CRC_ERROR,          ///< CRC 오류
        SAVEPATH_NOT_EXIST, ///< 지정된 저장 경로가 존재하지 않음.
        SAVEPATH_IS_NOT_DIR ///< 지정된 저장 경로는 폴더가 아닙니다.
    };
    Unhv3Status& operator=(const Status &status);
    QString getMessage();
    Status getStatusCode();

private:
    Status status_;

};

#endif // UNHV3STATUS_HPP
